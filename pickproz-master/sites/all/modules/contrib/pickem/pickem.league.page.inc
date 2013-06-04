<?php

/**
 * @file
 * Page callbacks for the main league dashboard.
 */


/**
 * Menu callback; presents the list of all weeks and games.
 */
function league_page($league) {
  if ($league) {
    return theme('league_page', array(
      'league' => $league,
    ));
  }
  else {
    return FALSE;
  }
}

function theme_league_page($variables) {
  $league = $variables['league'];

  drupal_set_title($league->name .' League');

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[1] = l($league->name, 'pickem/'. $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $o  = '<div class="pickem-league-page">';

  // Insert the weeks_leaderboard_block into league page.
  $block = block_load('pickem', 'weeks_leaderboard');
  $block_content = _block_render_blocks(array($block));
  $build = _block_get_renderable_array($block_content);
  $build['pickem_my_league_stats']['#block']->delta = 'league_page_weeks_leaderboard';
  $o .= drupal_render($build);

  // Insert the my_league_stats_block into league page.
  if (is_my_league($league)) {
    $block = block_load('pickem', 'my_league_stats');
    $block_content = _block_render_blocks(array($block));
    $build = _block_get_renderable_array($block_content);
    $build['pickem_my_league_stats']['#block']->delta = 'league_page_my_league_stats';
    $o .= drupal_render($build);
  }
  $o .= theme('league_messages', array('league' => $league));
  $o .= '</div>';
  return $o;
}


function theme_league_nav($variables) {
  $league = $variables['league'];
  global $user;

  $o = '';
  $o .=  '<ul>';

  if ( is_my_league($league) ) {
    $o .= '<li>'. l('My Picks', 'pickem/'. $league->lid .'/mypicks') .'</li>';
  }
  elseif (user_access('join leagues')) {
    $o .= '<li>'. l('Join League', 'pickem/'. $league->lid .'/join') .'</li>';
  }

  $cur_w = get_current_week($league, FALSE);
  if ($cur_w && $cur_w->wk_number <> '') {
    $o .= '<li>'. l('Pick Summary('. $cur_w->wk_number .')', 'pickem/'. $league->lid .'/summary/'. $cur_w->wid) .'</li>';
  }

  $o .= '<li>'. l('Standings', 'pickem/'. $league->lid .'/standings') .'</li>';

  if ( $league->rules_node_id != 0 ) {
    $o .= '<li>'. l('Rules and Regs', 'node/'. $league->rules_node_id) .'</li>';
  }

  if (module_exists("forum") && $league->forum_id != 0) {
    $o .= '<li>'. l('Talk and Taunting', 'forum/'. $league->forum_id) .'</li>';
  }

  if ( user_access('administer pickem') && $cur_w && $cur_w->wk_number <> '' ) {
    $o .= '<li>'. l('Update Scores', 'admin/pickem/scores/'. $league->lid . '/edit/' . $cur_w->wid) .'</li>';
  }

  $o .= '<li>Research<ul>';
  $o .= '<li>'. l('High/Low Chart', 'pickem/'. $league->lid .'/chart1') .'</li>';
  if ($league->partial_league == 0) {
    if ($league->show_schedule_grid == 1) {
      $o .= '<li>'. l('Schedule Grid', 'pickem/'. $league->lid .'/grid') .'</li>';
    }
    $o .= '<li>'. l('Team Pickability Chart', 'pickem/'. $league->lid .'/chart2') .'</li>';
    $o .= '<li>'. l('Team Win-Loss Stats', 'pickem/'. $league->lid .'/team_stats') .' / '. l('vPower', 'pickem/'. $league->lid .'/team_power') .'</li>';
  }
  $o .= '</ul></li></ul>';

  return theme('dash', array('title' => 'Navigation', 'content' => $o, 'class_wrapper' => 'league-page-navigation'));
}


function theme_league_messages($variables) {
  global $user;

  $league = $variables['league'];
  $output = '<div style="clear:both;">';
  if (module_exists("forum") && is_my_league($league) && $league->forum_id != 0) {
    $output .= l(t('Post new forum topic.'), 'node/add/forum/'. $league->forum_id);
  }

  $cur_w = get_current_week($league, TRUE);
  $output .= '<ul>';
  if ($cur_w != FALSE && $cur_w->first_gamestart != '' ) {
    if ( $cur_w->firstgame_dow <> "Sunday" ) {
      $output .= '<li class="warning">Warning: The first game is on <span style="text-decoration:underline;">'. strtoupper($cur_w->firstgame_dow) .'</span>.</li>';
    }
    if ($league->pick_lock == PICKEM_PICK_LOCK_WEEK) {
      $output .= '<li class="warning">Week '. ($cur_w->wk_number) .' picks due by '. format_date($cur_w->first_gamestart, 'medium');
      if (module_exists("jstimer")) {
        $output .= ',<br/>' . get_countdown_timer($cur_w);
      }
      $output .= '</li>';
    }
  }

  if (module_exists("forum") && is_my_league($league) && $league->forum_id != 0) {
    $topics = forum_get_topics($league->forum_id, 1, 4);
    $i = 1;
    $content='';
    foreach ($topics as $topic) {
      if ( $i > 4 ) {
        break;
      }
      $i++;
      if (isset($topic->last_reply)) {
        $content .= '<li>'. l('Re: ' . $topic->title .' - '. $topic->last_reply->name, 'node/'. $topic->nid  )  .' ('. format_date($topic->last_reply->created, 'short') .')</li>';
      }
      else {
        $content .= '<li>'. l($topic->title .' - '. $topic->name, 'node/'. $topic->nid  )  .' ('. format_date($topic->last_comment_timestamp, 'short') .')</li>';
      }
    }
    $output .= $content;
  }

  if (strlen($output) > 4) {
    $content = "<li>None</li>";
  }

  $output .= '</ul></div>';

  return theme('dash', array('title' => 'Messages', 'content' => $output, 'class_wrapper' => 'league-page-messages'));
}


function theme_dash($variables) {
  $title = $variables['title'];
  $content = $variables['content'];
  $class_wrapper = $variables['class_wrapper'];

  if ( isset($class_wrapper) ) {
    $output = "<div class=\"dash $class_wrapper\"><div class=\"title\">$title</div><div>$content</div></div>";
  }
  else {
    $output = "<div class=\"dash\"><div class=\"title\">$title</div><div>$content</div></div>";
  }
  return $output;
}
