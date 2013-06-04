<?php
/**
 * @file
 *
 * Provides the object definitions used by the win_loss scoring engine.
 */

/**
 * Definition for win_loss scoring engine.
 */
class pickem_scoring_win_loss implements pickem_scoring_engine_interface {

  // Properties
  private $props = array();

  // Magic overload method.
  public function __get($name) {
    if (array_key_exists($name, $this->props)) {
      return $this->props[$name];
    }
    trigger_error('Undefined property via __get(): ' . $name, E_USER_NOTICE);
    return NULL;
  }

  // Constructor
  public function __construct() {
    $this->props = array(
      'key' => PICKEM_TYPE_WIN_LOSS,
      'name' => t('Pick the winner'),
      'description' => t('Simply pick the winner of each game.'),
      'features' => array('pick'=>1, 'pick_score'=>0, 'with_spread'=>0, 'with_last_weekly_score'=>0),
    );
  }

  /**
   * Calculate user totals for this scoring system.
   *
   * @param $totals
   *   A deep array to store user totals (from get_user_totals() in pickem.inc)
   * @param $row
   *   A row object consisting of game and pick data (from get_user_totals() in pickem.inc)
   * @return
   *   NULL
   */
  function update_totals(&$totals=FALSE, &$game=FALSE) {

    if (isset($game->h_score) && isset($game->v_score)) {
      $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = 0;

      // They didn't pick, although there shouldn't be a record in the database then.
      if (empty($game->winnerpick_id)) {
        return;
      }

      // They picked the home winner.
      else if ($game->h_score > $game->v_score && $game->winnerpick_id == $game->h_id) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
        $totals[$game->uid]['total'] += $game->wk_points;
      }

      // They picked the visitor winner.
      else if ($game->v_score > $game->h_score && $game->winnerpick_id == $game->v_id) {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
        $totals[$game->uid]['total'] += $game->wk_points;
      }

      // Tie
      else if ($game->v_score == $game->h_score && $game->h_score <> "") {
        $totals[$game->uid]['weeks'][$game->wid]['games'][$game->gid] = $game->wk_points;
        $totals[$game->uid]['weeks'][$game->wid]['total'] += $game->wk_points;
        $totals[$game->uid]['total'] += $game->wk_points;
      }
    }
  }

  function update_totals_final(&$totals=FALSE) {

  }

}



