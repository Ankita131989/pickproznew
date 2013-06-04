<?php

/**
 * @file
 * Page callbacks and theme functions for the schedule grid page.
 */


function schedule_grid_page($league) {
  drupal_set_title("Schedule Grid");

  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l($league->name, 'pickem/'. $league->lid);
  drupal_set_breadcrumb($breadcrumb);

  $output = '';
  $cache_key = 'pickem_page_grid_'. $league->gsid;
  if ( ($cache = cache_get($cache_key)) && !empty($cache->data) ) {
    drupal_add_js('misc/tableheader.js');
    $output = $cache->data;
  }
  else {

    $games = array();
    $result = db_query("SELECT g.gid, g.wid, h.tm_abbrev as home, v.tm_abbrev as visitor, g.h_score, g.v_score FROM {pickem_games} g inner join {pickem_teams} h on h.tid=g.h_id inner join {pickem_teams} v on v.tid=g.v_id where g.gsid=:gsid order by g.gamestart", array(':gsid' => $league->gsid));
    foreach ($result as $record) {
      if ($record->h_score > $record->v_score) {
        // Home Victory
        $games[$record->home][$record->wid] = array(
          "op" => $record->visitor,
          "w-l" => "win",
          "h-a" => "home",
          "score" => $record->h_score .'-'. $record->v_score
        );
        $games[$record->visitor][$record->wid] = array(
          "op" => $record->home,
          "w-l" => "loss",
          "h-a" => "away",
          "score" => $record->v_score .'-'. $record->h_score
        );
      }
      elseif ($record->v_score > $record->h_score) {
        // Visitor Victory
        $games[$record->home][$record->wid] = array(
          "op" => $record->visitor,
          "w-l" => "loss",
          "h-a" => "home",
          "score" => $record->h_score .'-'. $record->v_score
        );
        $games[$record->visitor][$record->wid] = array(
          "op" => $record->home,
          "w-l" => "win",
          "h-a" => "away",
          "score" => $record->v_score .'-'. $record->h_score
        );
      }
      elseif ($record->v_score == $record->h_score && $record->h_score <> "") {
        // TIE
        $games[$record->home][$record->wid] = array(
          "op" => $record->visitor,
          "w-l" => "tie",
          "h-a" => "home",
          "score" => $record->h_score .'-'. $record->v_score
        );
        $games[$record->visitor][$record->wid] = array(
          "op" => $record->home,
          "w-l" => "tie",
          "h-a" => "away",
          "score" => $record->v_score .'-'. $record->h_score
        );
      }
      else {
        // TBD, game not played
        $games[$record->home][$record->wid] = array(
          "op" => $record->visitor,
          "w-l" => "tbd",
          "h-a" => "home",
          "score" => 'N/A'
        );
        $games[$record->visitor][$record->wid] = array(
          "op" => $record->home,
          "w-l" => "tbd",
          "h-a" => "away",
          "score" => 'N/A'
        );
      }
    }
    ksort($games);

    $output = '<div class="pickem">Hover over completed game for the score.';

    $weeks = get_weeks(array('league_id' => $league->lid, 'playoffs' => 0));
    $header = array('&nbsp;');
    foreach ($weeks as $wid => $week) {
      $header[] = array('data' => $week->wk_number, 'title' => 'Week Number '. $week->wk_number);
    }

    $rows = array();
    foreach ($games as $tm => $teamsgames) {
      ksort($teamsgames);
      $row = array();
      $row[] = array('data' => $tm, 'class' => 'team_head');
      foreach ($weeks as $w => $w_data) {
        if (isset ($teamsgames[$w])) {
          $class = $teamsgames[$w]["h-a"];
          $class .= " ". $teamsgames[$w]["w-l"];
          if ($teamsgames[$w]["h-a"] == "away") {
            $teamsgames[$w]["op"] = '@'. $teamsgames[$w]["op"];
          }
          $row[] = array('data' => $teamsgames[$w]["op"], 'class' => $teamsgames[$w]["w-l"], 'title' => strtoupper($teamsgames[$w]["w-l"]) .' '. $teamsgames[$w]["score"]);
        }
        else {
          if ($w_data->playoffs == 1) {
            // playoffs, blank, used for teams that didn't make the playoffs
            $row[] = '&nbsp;';
          }
          else {
            // bye week
            $row[] = array('data' => 'bye', 'class' => 'bye', 'title' => 'Bye Week');
          }
        }
      }
      $rows[] = $row;
    }

    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('grid')))
    );
    $output .= '</div>';

    cache_set($cache_key, $output);
  }

  return $output;
}