<div class="view-content">
<table>
<tbody>
<th class = "views-field-field-league">Sports</th>
<!--<th class = "views-field-field-home-team">Home Team</th>
<th class = "views-field-field-away-team">AwayTeam</th>-->
<th class = "views-field-field-event">Event</th>
<th class = "views-field-field-time-played">Time Played</th>
<th class = "views-field-field-rating">Rating</th>
<th class = "views-field-field-pickwin">Pick winner</th>
<th class = "views-field-field-pick-sold">Picks Sold</th>
<th class ="views-field-field-purchase">Purchase this Pick</th>

<?php
//echo "<pre>";
//print_r($view);
//echo "</pre>";
//echo die();
global $user;
$uid = $user->uid;
$stmt = "SELECT count(ps.field_pick_sold_nid) AS c ,ht.field_home_team_value, 
at.field_away_team_value, lv.field_league_value, tp.field_time_played_value,rv.field_rating_value,pw.field_pick_winner_value
FROM field_data_field_pick_mid AS pm
JOIN field_data_field_match_id AS mi ON pm.field_pick_mid_value = mi.field_match_id_value
JOIN node AS n ON n.nid = pm.entity_id
AND n.uid =$uid
JOIN node AS d ON d.nid = mi.entity_id
AND n.uid =$uid
JOIN field_data_field_league AS lv ON lv.entity_id = d.nid
JOIN field_data_field_home_team AS ht ON ht.entity_id = d.nid
JOIN field_data_field_away_team AS at ON at.entity_id = d.nid
JOIN field_data_field_time_played AS tp ON tp.entity_id = d.nid
LEFT JOIN field_data_field_rating AS rv on rv.entity_id = n.nid
LEFT JOIN  field_data_field_pick_sold AS ps on ps.field_pick_sold_nid = n.nid
LEFT JOIN field_data_field_pick_winner AS pw on pw.entity_id = n.nid
GROUP BY ps.field_pick_sold_nid
LIMIT 0 , 3";
$query = db_query($stmt);
foreach($query as $result)
{
$home_team = $result ->field_home_team_value;  
$away_team = $result ->field_away_team_value;  
$league_value = $result ->field_league_value;  
$time_played = $result ->field_time_played_value;  
$rating = $result ->field_rating_value;
$pick_sold = $result -> c;
$pick_win = $result -> field_pick_winner_value;
?>
<tr>
<td class ="views-field-field-league"><?=$league_value ?></td>
<!--<td class ="views-field-field-home-team"><?=$home_team ?></td>
<td class ="views-field-field-away-team"><?=$away_team ?></td>-->
<td class ="views-field-field-event"><?= $home_team .'&nbsp; @ &nbsp;'. $away_team?></td>
<td class ="views-field-field-time-played"><?=$time_played ?></td>
<td class ="views-field-field-rating"><?= $rating ?></td>
<td class = "views-field-field-pickwin"><?=$pick_win ?></td>
<td class ="views-field-field-pick-sold"><?=$pick_sold?></td>
<td class ="views-field-field-purchase"><a href = '#'>Buy</a></td>
</tr>
<?php } ?>
</tbody>
</table>
</div>





