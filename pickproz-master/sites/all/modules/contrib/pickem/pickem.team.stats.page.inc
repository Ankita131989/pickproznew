<?php

/**
 * @file
 * Page callbacks and theme functions for the team stats page.
 */


function team_stats_page($league) {
  drupal_set_title('Team Stats');

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/'. $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  return theme('team_stats', array('league' => $league));
}


function theme_team_stats($variables) {
  $league = $variables['league'];

  $teams = get_teams(array('gameset_id' => $league->gsid));
  foreach ($teams as $tid => $team) {
    $team->wins = 0;
    $team->losses = 0;
    $team->ties = 0;
    $team->games_played = 0;
    $team->home_wins = 0;
    $team->home_losses = 0;
    $team->vis_wins = 0;
    $team->vis_losses = 0;
    $team->win_percent = 0.0;
    $team->pts_for = 0;
    $team->pts_against = 0;
  }

  $games = get_games(array('gameset_id' => $league->gsid));
  foreach ($games as $gid => $game) {
    if ( ! isset($game->h_score) || ! isset($game->v_score) ) {
      continue;
    }

    if ( $game->h_score > $game->v_score ) {
      $teams[$game->h_id]->wins++;
      $teams[$game->h_id]->home_wins++;
      $teams[$game->v_id]->losses++;
      $teams[$game->v_id]->vis_losses++;
    }
    elseif ( $game->h_score < $game->v_score ) {
      $teams[$game->v_id]->wins++;
      $teams[$game->v_id]->vis_wins++;
      $teams[$game->h_id]->losses++;
      $teams[$game->h_id]->home_losses++;
    }
    elseif ( $game->h_score == $game->v_score ) {
      $teams[$game->v_id]->ties++;
      $teams[$game->h_id]->ties++;
    }

    $teams[$game->h_id]->games_played++;
    $teams[$game->v_id]->games_played++;

    if ( $teams[$game->h_id]->wins + $teams[$game->h_id]->losses > 0 ) {
      $teams[$game->h_id]->win_percent = $teams[$game->h_id]->wins / ( $teams[$game->h_id]->wins + $teams[$game->h_id]->losses );
    }

    if ( $teams[$game->v_id]->wins + $teams[$game->v_id]->losses > 0 ) {
      $teams[$game->v_id]->win_percent = $teams[$game->v_id]->wins / ( $teams[$game->v_id]->wins + $teams[$game->v_id]->losses );
    }

    $teams[$game->v_id]->pts_for += $game->v_score;
    $teams[$game->v_id]->pts_against += $game->h_score;

    $teams[$game->h_id]->pts_for += $game->h_score;
    $teams[$game->h_id]->pts_against += $game->v_score;

  }

  // setup sort
  $header = array(
    array('data' => t('Team'), 'field' => 'team'),
    array('data' => t('W')),
    array('data' => t('L')),
    array('data' => t('T')),
    array('data' => t('PCT'), 'field' => 'pct', 'sort' => 'desc'),
    array('data' => t('PF')),
    array('data' => t('PA')),
    array('data' => t('HOME')),
    array('data' => t('ROAD'))
  );

  $ts = tablesort_get_order($header);
  $ts['sort'] = tablesort_get_sort($header);

  if ( $ts['name'] == 'PCT' ) {
    uasort($teams, "sort_team_stats_pct_". $ts['sort']);
  }
  elseif ( $ts['name'] == 'Team' ) {
    uasort($teams, "sort_team_stats_team_". $ts['sort']);
  }

  // render team data
  $rows = array();
  foreach ($teams as $tid => $team) {

    $team_link = $teams[$tid]->long_name;
    $team_link = l($team_link, 'pickem/'. $league->lid .'/chart3/'. $tid);

    $rows[] = array(
      array('data' => $team_link),
      array('data' => $teams[$tid]->wins, 'style' => 'width:20px;'),
      array('data' => $teams[$tid]->losses, 'style' => 'width:20px;'),
      array('data' => $teams[$tid]->ties, 'style' => 'width:20px;'),
      array('data' => sprintf("%.3f", $teams[$tid]->win_percent), 'style' => 'width:55px;'),

      array('data' => $teams[$tid]->pts_for, 'style' => 'width:20px;'),
      array('data' => $teams[$tid]->pts_against, 'style' => 'width:20px;'),

      array('data' => $teams[$tid]->home_wins .'-'. $teams[$tid]->home_losses, 'style' => 'width:48px;'),
      array('data' => $teams[$tid]->vis_wins .'-'. $teams[$tid]->vis_losses, 'style' => 'width:48px;')
    );
  }

  $o = theme('table', array('header' => $header, 'rows' => $rows, 'style' => '100%;'));

  return $o;
}


// bunch of sort routines for team_stats
// should really simplify this
function sort_team_stats_pct_desc($a, $b) {
  if ($a->win_percent > $b->win_percent) {
    return -1;
  }
  elseif ($a->win_percent < $b->win_percent) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_pct_asc($a, $b) {
  if ($a->win_percent < $b->win_percent) {
    return -1;
  }
  elseif ($a->win_percent > $b->win_percent) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_team_desc($a, $b) {
  if ($a->long_name > $b->long_name) {
    return -1;
  }
  elseif ($a->long_name < $b->long_name) {
    return 1;
  }
  else {
   return 0;
  }
}
function sort_team_stats_team_asc($a, $b) {
  if ($a->long_name < $b->long_name) {
    return -1;
  }
  elseif ($a->long_name > $b->long_name) {
    return 1;
  }
  else {
   return 0;
  }
}
