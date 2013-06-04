<?php
/**
 * @file
 * Provides methods for created and modifying tinypass enabled tags
 */
/**
 * Tinypass
 * ------------------------------------------------------------------------
 * @author    TinyPass, Inc
 * @copyright Copyright (C) 2011 tinypass.com. All Rights Reserved.
 * @license - http://www.gnu.org/licenses/lgpl-3.0.html GNU/LGPL
 * Websites: http://tinypass.com
 * Technical Support: http://developer.tinypass.com
 */

function tinypass_tag_list_page() {
	$output = '';
	$result = db_query("SELECT tm.*, term.name, term.tid FROM
												{tinypass_tag_meta} tm,
												{taxonomy_vocabulary} v,
												{taxonomy_term_data} term
												WHERE v.vid = term.vid AND tm.tid = term.tid ");
	$rows = array();

	foreach ($result as $row) {
		$row = (array)$row;

		$data = array();
		$data['term'] = $row['name'];
		$data['tid'] = $row['tid'];
		$data['meta_id'] = $row['meta_id'];
		$data['details'] = '';

		$data['enabled'] = t('no');
		if ($row['enabled'])
			$data['enabled'] = t('yes');

		$meta = unserialize($row['data']);

		$data['resource_name'] = isset($meta['resource_name']) ? $meta['resource_name'] : '';

		$options = new TinyPassOptions($meta, 1);

		$data['details'] = tinypass_options_overview($options);

		$rows[] = $data;

	}



	drupal_set_title(t("TinyPass configuration for tags/terms "));
	$output .= theme('tinypass_tag_list_display', array('rows' => $rows));
	return $output;
}

/**
 *
 */
function tinypass_edit_tag_form_validate($form, &$form_state) {
	_tinypass_parse_meta_data($form, $form_state);
}

/**
 *
 */
function _tinypass_parse_meta_data(&$form, &$form_state) {

	$data = array();
	$pos = array();
	$tp_error = FALSE;
	$post_data = $form_state['values'];


	$data['enabled'] = isset($post_data['tp_enable_for_tag'])?$post_data['tp_enable_for_tag']:"0";
	$data['meta_id'] = $post_data['meta_id'];

	$meta = array();
	$meta['resource_name'] = $post_data['tp_resource_name'];

	//metered
	if ($post_data['metered'] == 'off') {
		$meta['metered'] = 'off';
	}
	elseif ($post_data['metered'] == 'time') {
		$meta['metered'] = 'time';
		$meta['m_tp'] = $post_data['m_tp'];
		$meta['m_tp_type'] = $post_data['m_tp_type'];
		$meta['m_lp'] = $post_data['m_lp'];
		$meta['m_lp_type'] = $post_data['m_lp_type'];

		if ($meta['m_tp'] == '')
			form_set_error("m_tp", t('Trial period cannot be empty'));

		if (!is_numeric($meta['m_tp']))
			form_set_error("m_tp", t('Trial period must be a valid number'));

		if ($meta['m_lp'] == '')
			form_set_error("m_lp", t('Lockout period cannot be empty'));

		if (!is_numeric($meta['m_lp']))
			form_set_error("m_lp", t('Lockout period must be a valid number'));

	}
	else {

		$meta['metered'] = 'count';
		$meta['m_maa'] = $post_data['m_maa'];
		$meta['m_lp'] = $post_data['m_lp'];
		$meta['m_lp_type'] = $post_data['m_lp_type'];

		if ($meta['m_maa'] == '')
			form_set_error("m_maa", t('Max views cannot be empty'));

		if (!is_numeric($meta['m_maa']))
			form_set_error("m_maa", t('Max views must be a valid number'));

		if ($meta['m_lp'] == '')
			form_set_error("m_lp", t('Lockout period cannot be empty'));

		if (!is_numeric($meta['m_lp']))
			form_set_error("m_lp", t('Lockout period must be a valid number'));

	}

	//price options
	$post_data["tp_option_enabled0"] = "1";
	for ($i = 0; $i < 3; $i++) {

		$podata['en'] = (isset($post_data["tp_option_enabled$i"])) ? $post_data["tp_option_enabled$i"] : "0";

		if ($podata['en'] == "1") {

			$podata['p'] = $post_data["tp_price$i"];
			if ($podata['p'] == "") {
				form_set_error("tp_price$i", t('Price cannot be empty'));
			}
			if (!is_numeric($podata['p'])) {
				form_set_error("tp_price$i", t('Price must be a valid number'));
			}

			$podata['st'] = trim($post_data["tp_start_time$i"]);
			$podata['et'] = trim($post_data["tp_end_time$i"]);
			$podata['ap'] = $post_data["tp_access_period$i"];
			$podata['apt'] = $post_data["tp_access_period_type$i"];

			if (!is_numeric($podata['ap']) && $podata['ap'] != '') {
				$tp_error = TRUE;
				form_set_error("tp_access_period$i", t('Access Period must be a valid number'));
			}
			$podata['cpt'] = trim($post_data["tp_caption$i"]);

			_tinypass_unset_if_empty($podata, 'cpt');
			_tinypass_unset_if_empty($podata, 'st');
			_tinypass_unset_if_empty($podata, 'et');

			$pos[$i] = $podata;
		}
	}

	$meta['pos'] = $pos;

	$data['meta'] = $meta;


	$term_text = $post_data['term'];
	$term_text = preg_replace('/\(\d?\)/', '', $term_text);
	$values = preg_split('/\//', $term_text);

	if (count($values) != 2) {
		form_set_error("term", "Invalid term/taxnomy value for ");
	}
	else {
		$data['vname'] = $values[0];
		$data['tname'] = $values[1];

		$r = db_query("SELECT term.tid FROM {taxonomy_term_data} term, {taxonomy_vocabulary} v
							WHERE v.vid = term.vid
							AND v.name = :vname
							AND term.name = :tname", array(':vname' => $data['vname'], ':tname' => $data['tname']));
		if($r->rowCount() == 0) {
			form_set_error("term", "Invalid term/taxnomy value for ");
		} else {
			$row = $r->fetchAssoc();
			$data['tid'] = $row['tid'];
		}

	}

	return $data;

}


function tinypass_edit_tag_form_submit($form, &$form_state) {

	$data = _tinypass_parse_meta_data($form, $form_state);

	$record = array();
	$record['tid'] = $data['tid'];
	$record['enabled'] = $data['enabled'];
	$record['data'] = serialize($data['meta']);
	$record['meta_id'] = $data['meta_id'];


	$update = array();
	if (isset($record['meta_id']) && $record['meta_id'] > 0) {
		$update = 'meta_id';
	}

	drupal_write_record("tinypass_tag_meta", $record, $update);

	drupal_set_message(t("Tag details saved"));

	$form_state['redirect'] = 'admin/content/tinypass/tag_list';

	return;
}

/**
 *
 * @param <type> $form_state
 * @param <type> $form_data
 * @return <type>
 */
function tinypass_edit_tag_form($form_state, $form_data = NULL, $data = array()) {

	drupal_add_js(drupal_get_path('module', 'tinypass') . '/tinypass.js');
	drupal_add_css(drupal_get_path('module', 'tinypass') . '/tinypass.css');

	$form = array();

	$enabled = 1;
	$term = 1;
	$tid = -1;
	$meta_id = '';
	$resource_name = '';

	$metered = 'off';

	$lockout_period = '';
	$lockout_period_type = '';

	$trial_period = '';
	$trial_period_type = '';

	$view_limit = '';

	$meta = array();

	if(isset($data['data'])) {
		$meta = $data['data'];
	}

	if (isset($data['enabled']))
		$enabled = $data['enabled'];

	if (isset($data['tid']))
		$tid = $data['tid'];

	if (isset($data['meta_id']))
		$meta_id = $data['meta_id'];

	if (isset($meta['resource_name']))
		$resource_name = $meta['resource_name'];

	if (isset($meta['m_lp']))
		$lockout_period = $meta['m_lp'];

	if (isset($meta['m_maa']))
		$view_limit = $meta['m_maa'];

	if (isset($meta['m_lp_type']))
		$lockout_period_type = $meta['m_lp_type'];

	if (isset($meta['m_tp']))
		$trial_period = $meta['m_tp'];

	if (isset($meta['m_tp_type']))
		$trial_period_type = $meta['m_tp_type'];


	$vars = array();
	$vars['tp_metered_count_class'] = 'hide';
	$vars['tp_metered_time_class'] = 'hide';
	if (isset($meta['metered'])) {

		$metered = $meta['metered'];

		if ($metered == 'count') {
			$vars['tp_metered_count_class'] = '';
			$vars['tp_metered_time_class'] = 'hide';
		}
		elseif ($metered == 'time') {
			$vars['tp_metered_count_class'] = 'hide';
			$vars['tp_metered_time_class'] = '';
		}
	}
	$form['#vars'] = $vars;


	$times = array('minute' => 'minute(s)', 'hour' => 'hour(s)', 'day' => 'day(s)', 'week' => 'week(s)', 'month' => 'month(s)');

	$result = db_query("SELECT t.tid, t.name, v.name vname FROM {taxonomy_term_data} t, {taxonomy_vocabulary} v WHERE t.vid = v.vid AND t.tid = :TID ", array(":TID" => $tid));

	$term = '';
	if ($result->rowCount()) {
		$row = $result->fetchAssoc();
		$term = $row['vname'] . '/' . $row['name'] . "(" . $row['tid'] .")";
	}


	$form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

	$form['tinypass'] = array(
					'#type' => 'fieldset',
					'#title' => t('Details'),
					'#collapsible' => FALSE,
					'#collapsed' => FALSE,
					'#weight' => 30,
	);

	$form['tinypass']['tp_enable_for_tag'] = array(
					'#type' => 'checkbox',
					'#title' => t('Enable TinyPass for this tag/term'),
					'#default_value' => $enabled,
					'#description' => 'Disabling will cause TinyPass to be skipped',
					'#weight' => -1,
	);

	$form['tinypass']['term'] = array(
					'#type' => 'textfield',
					'#title' => t('Term'),
					'#autocomplete_path' => 'admin/tinypass/taxonomy_autocomplete',
					'#default_value' => $term,
					'#description' => t('TinyPass will be enabled for all content that has been assigned the following term.  <b>Only one term may be selected</b>'),
					'#required' => TRUE,
					'#size' => 30,
	);

	$form['tinypass']['tp_resource_name'] = array(
					'#type' => 'textfield',
					'#title' => t('Display name'),
					'#default_value' => $resource_name,
					'#description' => t('This will be displayed on the TinyPass popup - This is the friendly name of what your users are purchasing'),
					'#maxlength' => 255,
					'#required' => TRUE,
					'#size' => 60,
	);

	$form['tinypass']['metered'] = array(
					'#type' => 'select',
					'#default_value' => $metered,
					'#attributes' => array(
									'onchange' => 'tinypass.showMeteredOptions(this)'),
					'#options' => array('off' => 'Disabled', 'count' => 'View Based',  'time' => 'Time Based' )
	);


	//View Limit
	$form['tinypass']['m_maa'] = array(
					'#type' => 'textfield',
					'#default_value' => $view_limit,
					'#maxlength' => 5,
					'#size' => 5,
	);

	//Lockout Period
	$form['tinypass']['m_lp'] = array(
					'#type' => 'textfield',
					'#default_value' => $lockout_period,
					'#maxlength' => 5,
					'#size' => 5,
	);

	//Lockout Period
	$form['tinypass']['m_lp_type'] = array(
					'#type' => 'select',
					'#default_value' => $lockout_period_type,
					'#options' => $times,
	);


	//Trial Period
	$form['tinypass']['m_tp'] = array(
					'#type' => 'textfield',
					'#default_value' => $trial_period,
					'#maxlength' => 5,
//					'#theme' => 'lockout',
					'#size' => 5,
	);
	//Trial Period Type
	$form['tinypass']['m_tp_type'] = array(
					'#type' => 'select',
					'#default_value' => $trial_period_type,
					'#options' => $times,
	);


	$form['tinypass']['meta_id'] = array(
					'#type' => 'hidden',
					'#value' => $meta_id,
	);

	$form['cancel'] = array(
					'#type' => 'submit',
					'#value' => 'cancel',
					'#attributes' => array('onClick' => 'history.go(-1);return false;'),
	);




	$pos = isset($meta['pos']) ? $meta['pos'] : array();
	for ($i = 0; $i < 3; $i++) {

		$price = '';
		$start_time = '';
		$end_time = '';
		$access_period = '';
		$caption = '';
		$apt = '';
		if (isset($pos[$i])) {
			if (isset($pos[$i]['p'])) {
				$price = $pos[$i]['p'];
			}

			if (isset($pos[$i]['st'])) {
				$start_time = trim($pos[$i]['st']);
			}

			if (isset($pos[$i]['et'])) {
				$end_time = trim($pos[$i]['et']);
			}

			if (isset($pos[$i]['ap'])) {
				$access_period = $pos[$i]['ap'];
			}

			if (isset($pos[$i]['apt'])) {
				$apt = $pos[$i]['apt'];
			}

			if (isset($pos[$i]['cpt'])) {
				$caption = $pos[$i]['cpt'];
			}

		}

		if ($i > 0) {
			$form['tinypass']['tp_option_enabled' . $i] = array(
							'#type' => 'hidden',
							'#default_value' => $price != '' && $price > 0 ? TRUE : FALSE,
							'#title' => check_plain("Price Option" . ($i+1)),
							'#attributes' => array('class' => array('tinypass_option_enabled')),
			);
		}


		$form['tinypass']['tp_price' . $i] = array(
						'#type' => 'textfield',
						'#title' => t("Price"),
						'#default_value' => $price,
						'#maxlength' => 5,
						'#size' => 5,
						'#description' => t(''),
		);


		$form['tinypass']['tp_start_time' . $i] = array(
						'#type' => 'date_popup',
//						'#date_type' => DATE_DATETIME,
						'#date_timezone' => date_default_timezone(),
						'#title' => t("Price start datetime"),
						'#size' => 10,
						'#default_value' => $start_time,
//						'#description' => t(''),
		);

		$form['tinypass']['tp_end_time' . $i] = array(
						'#type' => 'date_popup',
//						'#date_type' => DATE_DATETIME,
						'#date_timezone' => date_default_timezone(),
						'#title' => t("Price end datetime"),
						'#size' => 10,
						'#default_value' => $end_time,
//						'#description' => t(''),
		);

		$form['tinypass']['tp_access_period' . $i] = array(
						'#type' => 'textfield',
						'#size' => 4,
						'#maxlength' => 4,
						'#default_value' => $access_period,
						'#value1' => $access_period
		);

		$form['tinypass']['tp_access_period_type' . $i] = array(
						'#type' => 'select',
						'#default_value' => $apt,
						'#options' => array('hour' => 'hour(s)', 'day' => 'day(s)',  'week' => 'week(s)', 'month' => 'month(s)')
		);


		$form['tinypass']['tp_caption' . $i] = array(
						'#type' => 'textfield',
						'#title' => t("Custom Caption"),
						'#default_value' => $caption,
						'#maxlength' => 30,
						'#size' => 30,
						'#description' => t(''),
		);

	}

	return $form;
}


//function template_preprocess_tinypass_edit_tag_form(&$variables) {
//	tinypass_debug($variables);
//	exit;
//$variables += $variables['form']['#vars'];
//}


function tinypass_new_tag_page() {
	return drupal_get_form('tinypass_edit_tag_form');
}

function tinypass_edit_tag_page($id) {
	$result = db_query('SELECT * FROM {tinypass_tag_meta} WHERE meta_id = :id', array(":id" => $id))->fetchAssoc();
	$result['data'] = unserialize($result['data']);
	return drupal_get_form('tinypass_edit_tag_form', $result);
}

function tinypass_remove_tag_page($meta_id) {
	db_query("DELETE FROM {tinypass_tag_meta} WHERE meta_id = :id ", array(":id" => $meta_id));
	drupal_set_message(t("Tag details deleted"));
	drupal_goto(url('admin/content/tinypass/tag_list'));
}

/**
 * Page callback to show all nodes with TP configured
 */
function tinypass_articles_page() {
	drupal_set_title(t("TinyPass enabled content"));

	//$result = db_query("SELECT distinct n.nid, nm.enabled en1, tm.enabled en2, n.type, n.title FROM {node} n
	//												LEFT JOIN {tinypass_node_meta} nm
	//														ON n.nid = nm.nid
	//												LEFT OUTER join {term_node}
	//														ON term_node.nid = n.nid
	//												LEFT JOIN {tinypass_tag_meta} tm
	//														ON term_node.tid = tm.tid WHERE (tm.enabled = 1 OR nm.enabled = 1)", array(), 75);

	$query = db_select('node', 'n');//->extend("PagerDefault");
	$query->distinct();
	$query->addField('n', 'nid');
	$query->addField('nm', 'enabled', 'en1');
	$query->addField('tm', 'enabled', 'en2');
	$query->addField('n', 'type');
	$query->addField('n', 'title');
	$query->join('tinypass_node_meta', 'nm', 'n.nid = nm.nid');
	$query->leftjoin('taxonomy_index', 'term_node', 'term_node.nid = n.nid');
	$query->join('tinypass_tag_meta', 'tm', 'term_node.tid = tm.tid');
	$or = db_or()->condition('tm.enabled', '1')->condition('nm.enabled', '1');
	$query->condition($or);

	$query = $query->extend('PagerDefault');
	$query->limit(50);


	//$value = $query->range('0', '100')->execute();
	$results = $query->execute();

	$rows = array();
	foreach($results as $result) {
		$row = (array)$result;
		$r = array();

		$r['ID'] = $row['nid'];
		$r['name'] = $row['title'];
		$r['type'] = $row['type'];

		$e = array();
		if($row['en1'])
			$e[] = t('Node');
		if($row['en2'])
			$e[] = t('Taxonomy');

		$r['enabled'] = implode(',', $e);

		$r['action'] = l(t('Edit'), 'node/' . $row['nid'] . '/edit/');

		$rows[] = $r;
	}

	$output = "";

	$header = array(t('ID'), t('Title'), t('Enabled For'), t('Type'), t('Action'));

//	module_load_include('inc', 'node', 'node.admin');
//	$output .= drupal_render(node_filter_form());

	$output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'my-module-table')));
	$output .= theme('pager');
	return $output;

}

/**
 * Helper function for autocompletion
 */
function tinypass_taxonomy_autocomplete($string = '') {

	// Fetch last tag
	$last_string = trim(array_pop(drupal_explode_tags(trim($string))));
	$matches = array();
	if ($last_string != '') {
		$result = db_query("SELECT t.tid, t.name, v.name vname FROM {taxonomy_term_data} t, {taxonomy_vocabulary} v WHERE t.vid = v.vid AND t.name LIKE :search ", array(':search' => '%' . db_like($last_string) . '%'));

		foreach($result as $tag) {
			$n = $tag->name;
			// Commas and quotes in terms are special cases, so encode 'em.
			if (strpos($tag->name, ',') !== FALSE || strpos($tag->name, '"') !== FALSE) {
				$n = '"'. str_replace('"', '""', $tag->name) .'"';
			}
			$matches[$tag->vname . "/" . $tag->name . "(" . $tag->tid . ")"] = check_plain($tag->name);
		}

	}

	echo drupal_json_encode($matches);
}
