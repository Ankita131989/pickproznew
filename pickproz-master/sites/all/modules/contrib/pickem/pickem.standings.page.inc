<?php

/**
 * @file
 * Page callbacks for display league standings.
 */


/**
 * Menu callback; presents the list of all weeks and games.
 */
function standings_page($league) {
  return theme('standings', array('league' => $league));
}


/**
 * Display the league standings.
 */
function theme_standings($variables) {
  $league = $variables['league'];

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/'. $league->lid);
  drupal_set_breadcrumb($breadcrumb);
  drupal_set_title("Standings");
  drupal_add_library('system', 'drupal.collapse');

  $totals = get_user_totals($league);
  $week_totals = get_totals_by_week($league);

  $flip = array('even' => 'odd', 'odd' => 'even');
  $class = 'even';

  $output = '<div class="pickem">Click on week numbers to see weekly pick summary.';

  // Regular season table.
  $reg_weeks = get_weeks(array('gameset_id' => $league->gsid, 'playoffs' => 0));
  if (count($reg_weeks) > 0) {
    $output .= '<div><fieldset class="collapsible"><legend><span class="fieldset-legend"><a href="#">Regular Season</a></span></legend>';
    $output .= '<div class="fieldset-wrapper"><table class="standings"><tr><th>Overall Rank</th>';

    $wk_summary = array();
    $output .= '<th colspan="2">TOT</th>';
    foreach ($reg_weeks as $wid => $week) {
      $output .= '<th class="week-numbers">'. l($week->wk_number, 'pickem/'. $league->lid ."/summary/$wid") .'</th>';
    }
    $output .= "</tr>";

    foreach ($totals as $uid => $tot) {
      $class = $flip[$class];
      $output .= '<tr class="'. $class .'">';
      $output .= '<td class="users">'. $tot['ranking'] .'.&nbsp;&nbsp;'. theme('username', array('account' => $tot['user_obj'])) .'</td>';

      if ($tot['pts_back'] == 0) {
        $output .= '<td class="totals">'. $tot['total'] .'</td><td class="gamesback">&nbsp;</td>';
        $high = $tot['total'];
      }
      else {
        $output .= '<td class="totals">'. $tot['total'] .'</td><td class="gamesback">+'. $tot['pts_back'] .'</td>';
      }

      foreach ($reg_weeks as $wid => $week) {
        // Week does not yet have any scores.
        if (!isset($tot['weeks'][$wid])) {
          $output .= '<td>&nbsp;</td>';
        }
        else if ($tot['weeks'][$wid]['picked_games'] < $tot['weeks'][$wid]['weeks_games']) {
          $output .= '<td title="' . t('Did not pick every game') . '" class="weekly skipped-pick">'. $tot['weeks'][$wid]['total'] .'</td>';
        }
        else if ($tot['weeks'][$wid]['total'] == $week_totals[$wid]["min"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $output .= '<td class="weekly weekly-low">'. $tot['weeks'][$wid]['total'] .'</td>';
        }
        else if ($tot['weeks'][$wid]['total'] == $week_totals[$wid]["max"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $output .= '<td class="weekly weekly-high">'. $tot['weeks'][$wid]['total'] .'</td>';
        }
        else {
          $output .= '<td class="weekly">'. $tot['weeks'][$wid]['total'] .'</td>';
        }
      }
      $output .= "</tr>";
    }
    $output .= '</table></div></fieldset></div>';
  }

  // Playoff table.
  $playoff_weeks = get_weeks(array('gameset_id' => $league->gsid, 'playoffs' => 1));
  if ( count($playoff_weeks) > 0 ) {
    $output .= '<div><fieldset class="collapsible"><legend><span class="fieldset-legend"><a href="#">Playoffs</a></span></legend>';
    $output .= '<div class="fieldset-wrapper"><table class="standings">';

    $output .= '<tr><th>Overall Rank</th><th colspan="2">TOT</th>';
    foreach ($playoff_weeks as $wid => $week) {
      $output .= '<th class="week-numbers">'. l('Week&nbsp;'. $week->wk_number .'<br/>'. $week->wk_name, 'pickem/'. $league->lid ."/summary/$wid", array('html' => TRUE)) .'</th>';
    }
    $output .= "</tr>";
    foreach ($totals as $uid => $u) {
      $class = $flip[$class];
      $output .= '<tr class="'. $class .'">';
      $output .= '<td class="users">'. $u['ranking'] .'.&nbsp;&nbsp;'. theme('username', array('account' => $u['user_obj'])) .'</td>';

      if ($u['pts_back'] == 0) {
        $output .= '<td class="totals">'. $u['total'] .'</td><td class="gamesback">&nbsp;</td>';
        $high = $u['total'];
      }
      else {
        $output .= '<td class="totals">'. $u['total'] .'</td><td class="gamesback">+'. $u['pts_back'] .'</td>';
      }
      foreach ($playoff_weeks as $wid => $week) {
        if ($u['weeks'][$wid]['picked_games'] < $u['weeks'][$wid]['weeks_games']) {
          $output .= '<td title="' . t('Did not pick every game') . '" class="weekly skipped-pick">'. $u['weeks'][$wid]['total'] .'</td>';
        }
        else if ($u['weeks'][$wid]['total'] == $week_totals[$wid]["min"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $output .= '<td class="weekly weekly-low">'. $u['weeks'][$wid]['total'] .'</td>';
        }
        else if ($u['weeks'][$wid]['total'] == $week_totals[$wid]["max"] && $week_totals[$wid]["min"] != $week_totals[$wid]["max"]) {
          $output .= '<td class="weekly weekly-high">'. $u['weeks'][$wid]['total'] .'</td>';
        }
        else {
          $output .= '<td>'. $u['weeks'][$wid]['total'] .'</td>';
        }
      }
      $output .= "</tr>";
    }
    $output .= '</table></div></fieldset></div>';
  }

  $output .= '</div>';
  return $output;
}
