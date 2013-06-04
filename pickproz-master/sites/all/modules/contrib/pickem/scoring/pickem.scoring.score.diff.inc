<?php
/**
 * @file
 *
 * Provides the object definitions used by the differential scoring scoring engine.
 */

/**
 * Definition for differential scoring scoring engine.
 */
class pickem_scoring_score_diff implements pickem_scoring_engine_interface {

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
      'key' => PICKEM_TYPE_SCORE_DIFF,
      'name' => t('Score differential'),
      'description' => t('Pick the score of each game and get points for correct score differential and winner.'),
      'features' => array('pick'=>0, 'pick_score'=>1, 'with_spread'=>0, 'with_last_weekly_score'=>0),
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
  function update_totals(&$totals=FALSE, &$row=FALSE) {
    $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = 0;

    if (isset($row->h_score) && isset($row->v_score)) {

      // They didn't pick, although there shouldn't be a record in the database then.
      if (!isset($row->pick_h_score) || !isset($row->pick_v_score)) {
        return;
      }

      // Picked the correct winner or tie (1 point).
      if ($row->h_score > $row->v_score && $row->pick_h_score > $row->pick_v_score ||
          $row->v_score > $row->h_score && $row->pick_v_score > $row->pick_h_score ||
          $row->v_score == $row->h_score && $row->pick_v_score == $row->pick_h_score) {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] = 1;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += 1;
        $totals[$row->uid]['total'] += 1;
      }

      // Picked the correct score differential (1 more point).
      if ($row->h_score - $row->v_score == $row->pick_h_score - $row->pick_v_score) {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] += 1;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += 1;
        $totals[$row->uid]['total'] += 1;
      }

      // Picked the exact match score (2 more points).
      if ($row->h_score == $row->pick_h_score && $row->v_score == $row->pick_v_score) {
        $totals[$row->uid]['weeks'][$row->wid]['games'][$row->gid] += 2;
        $totals[$row->uid]['weeks'][$row->wid]['total'] += 2;
        $totals[$row->uid]['total'] += 2;
      }
    }
  }


  function update_totals_final(&$totals=FALSE) {

  }
}
