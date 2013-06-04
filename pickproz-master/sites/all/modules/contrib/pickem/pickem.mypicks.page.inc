<?php

/**
 * @file
 * Page callbacks for adding, editing, and deleting user picks.
 */


/**
 * Menu callback; presents the list of all weeks and games.
 */
function mypicks_all_page($league) {
  return theme('mypicks_all', array('league' => $league));
}


/**
 * Display the list of all weeks and games.
 * The user would pick a week link from here to make their weekly picks.
 */
function theme_mypicks_all($variables) {
  global $_PICKEM_MY_LEAGUES;
  global $user;

  $league = $variables['league'];

  // Param check: user must be a member of this league to continue.
  if ( !is_my_league($league) ) {
    return drupal_not_found();
  }

  drupal_set_title("My Picks - ". $user->name);

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/'. $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $teams = get_teams(array('gameset_id' => $league->gsid));
  $games = get_games(array('league_id' => $league->lid), array('by_week' => 1, 'order_by' => 'week_ended, g.gamestart, g.gid'));


  $header = array(t('Week'), t('Games'));
  $rows = array();
  foreach ($games as $wid => $weeks_games) {
    $cols = array();
    $current_week = '';
    foreach ($weeks_games as $gid => $game) {

      # Figure out the matchup operator.
      if ($game->neutral_venue == 1) {
        $matchup = "vs ";
      }
      else {
        $matchup = "@";
      }

      // The first column should be the week number link.
      if ( count($cols) == 0 ) {
        $cols[] = array('data' => l('Week&nbsp;'. $game->wk_number, "pickem/$league->lid/mypicks/" . $wid, array('html' => TRUE)), 'class' => 'week');
      }
      $current_week .= '<div class="picks-float">' . $teams[$game->v_id]->short_name .'<br/>' . $matchup . $teams[$game->h_id]->short_name . '</div>';
    }
    $cols[] = $current_week;
    $rows[] = $cols;
  }
  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-mypicks-all'))));
}


function mypicks_week_form($form, &$form_state, $league, $week) {
  global $user;

  $form = build_picks_week_form($form, $form_state, $league, $week, $user);
  return $form;
}


function theme_mypicks_week_form($variables) {
  global $user;

  $form = $variables['form'];
  $league = league_load($form['league_id']['#value']);

  // Drupal sets the default timezone to be the user's
  //if available, otherwise, the system's.
  $timezone = date_default_timezone_get();

  $header = array();
  $header[] = t('Matchup');
  $header[] = t('Status');
  $header[] = t('Time(%pickem-timezone)', array('%pickem-timezone' => $timezone));

  $rows = array();
  $last = count(element_children($form['games']));
  $count = 0;
  foreach (element_children($form['games']) as $gid) {
    $count++;
    $cols = array();

    if (isset($form['games'][$gid]['pick'])) {
      if ($form['games'][$gid]['pick']['#default_value'] == '') {
        $cols[] = array('data'=>drupal_render($form['games'][$gid]['pick']), 'class'=>'no-pick');
      }
      else {
        $cols[] = array('data'=>drupal_render($form['games'][$gid]['pick']));
      }
    }
    else {
      $cols[] = drupal_render($form['games'][$gid]['pick_v_score']) . drupal_render($form['games'][$gid]['pick_h_score']);
    }

    $cols[] = drupal_render($form['games'][$gid]['pick_status']);

    $cols[] = drupal_render($form['games'][$gid]['gamestart']);
    $rows[] = $cols;
  }

  $output = $league->scoring_engine->description;
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('pickem-mypicks-week'))));
  $output .= drupal_render_children($form);
  return $output;
}


function mypicks_week_form_submit($form, &$form_state) {
  global $user;

  $league = league_load($form_state['values']['league_id']);
  $week = week_load($form_state['values']['week_id']);

  // You should not be able to save picks after lockup.
  if ($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) {
    drupal_set_title($week->wk_name .' picks are locked.');
    return t("You cannot change your picks now.");
  }

  // Check and see if the user wants to save to all their leagues that use the same gameset.
  $save_for_these_leagues = array();
  if ( $form_state['values']['apply_to_all_leagues'] == 1 ) {
    $save_for_these_leagues = get_my_leagues($league->gsid);
  }
  else {
    $save_for_these_leagues[] = $league;
  }

  foreach ($save_for_these_leagues as $l) {
    // Only save for league using the same scoring system.
    $success = FALSE;
    if ($l->scoring_type == $league->scoring_type) {
      foreach ($form_state['values']['games'] as $gid => $pick) {

        // Don't send a blank pick to the database.
        if (($league->scoring_engine->features['pick'] == 1 && $pick['pick'] == '') ||
          ($league->scoring_engine->features['pick_score'] == 1 && $pick['pick_v_score'] == '' && $pick['pick_h_score'] == '')) {
          continue;
        }

        $pick['gid'] = $gid;
        if (set_pick($user->uid, $l, $pick, 'user') == TRUE) {
          $success = TRUE;
        }
      }
      if ($success) {
        drupal_set_message(t('The picks have been saved for league <i>%league</i>.', array('%league' => $l->name)));
      }
    }
  }


}
