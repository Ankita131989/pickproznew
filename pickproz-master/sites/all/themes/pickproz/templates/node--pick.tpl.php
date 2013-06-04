<?php
error_reporting(-1);
$nid = arg(1);
//echo "<pre>";
//print_r($node);
//echo "</pre>";
//print drupal_render_children($content);
    $content_id  = $node->vid;
	$winner 	 = $node->field_pick_winner['und']['0']['value'];
	$pick_mid 	 = $node->field_pick_mid['und']['0']['value'];
	$pick_week 	 = $node->field_pick_of_the_week['und']['0']['value'];
	$pick_rating = $node->field_rating['und']['0']['value'];
	$win_points	 = $node->field_will_they_win_by_points['und']['0']['value'];

//echo $node->rdf_mapping[rdftype][0];

$pick_rating_value = ($pick_rating =='0') ? "n/a" : "$pick_rating";

echo "<div id='block-system-main' class='block block-system'><div class='block-inner clearfix'><div class='block-content content'>";
	echo "<section class='field field-name-field-pick-winner field-type-list-text field-label-inline clearfix'>
		<h2 class='field-label'>Pick Winner:&nbsp;</h2>
			<div class='field-items'>
				<div class='field-item even'>$winner</div>
			</div>
		</section>";
	echo"<section class='field field-name-field-pick-mid field-type-number-integer field-label-inline clearfix'>
		<h2 class='field-label'>Pick MID:&nbsp;</h2>
			<div class='field-items'>
				<div class='field-item even'>$pick_mid</div>
			</div>
	     </section>";
	echo "<section class='field field-name-field-pick-of-the-week field-type-list-text field-label-inline clearfix'>
		<h2 class='field-label'>Pick of The Week:&nbsp;</h2>
			<div class='field-items'>
				<div class='field-item even'>$pick_week</div>
			</div>
	     </section>";
	echo"<section class='field field-name-field-rating field-type-starrating-rating field-label-inline clearfix'>
		<h2 class='field-label'>Pick rating:&nbsp;</h2>
			<div class='field-items'>
				<div class='field-item even'>$pick_rating_value</div>
			</div>
	    </section>";
$query = "select a.field_away_line_value,h.field_home_line_value from `field_data_field_match_id`as i
join field_data_field_away_line as a on a.entity_id = i.entity_id
join field_data_field_home_line as h on h.entity_id = i.entity_id
join field_data_field_pick_mid as q on q.field_pick_mid_value  = i.field_match_id_value group by q.entity_id = $nid";


	    $sql = db_query($query);
	    foreach($sql as $result)
	    {
	        $away_line = $result->field_away_line_value;
	        $home_line = $result->field_home_line_value;
	    }
	     $a = substr("$away_line", 0, 1);
         $b =  substr("$home_line", 0, 1);
        
	  if($away_line){
	  if($a == '+')
    {
     $round_title = round($away_line);
    }
    else{
     $round_title = round($home_line);
     } 
       echo"<section class='form-item form-type-select form-item-field-will-they-win-by-points-und'>
       
		<h2 class='field-label'>Will they win by $round_title points:&nbsp;</h2>
			<div class='field-items'>
				<div class='field-item even'>$win_points</div>
			</div>
	    </section>";
	    }
	    

echo "</div></div></div>";
error_reporting (E_ALL & ~E_NOTICE & ~E_WARNING);
drupal_get_form (flag_auto_compare_team_result($pick_mid,$content_id,$winner));
?>
