<?php
/**
 * @file
 * TinyPass configuration settings
 */
/**
 * Form builder; Configure the Tinypass system.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function tinypass_admin_settings($form) {

  // Generate the form - settings applying to all patterns first
  $form['general'] = array(
          '#type' => 'fieldset',
          '#weight' => -20,
          '#title' => t('General settings'),
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
  );

  $form['general']['tinypass_enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable TinyPass plugin'),
          '#size' => 10,
          '#maxlength' => 10,
          '#default_value' => variable_get('tinypass_enabled', '1'),
  );

  $form['general']['tinypass_aid_sand'] = array(
          '#type' => 'textfield',
          '#title' => t('Application ID (Sandbox)'),
          '#size' => 10,
          '#maxlength' => 10,
          '#default_value' => variable_get('tinypass_aid_sand', 'W7JZEZFu2h'),
          '#description' => t('TinyPass Application ID that identifies your client application.  Retreived from your TinyPass merchant account'),
  );

  $form['general']['tinypass_secret_key_sand'] = array(
          '#type' => 'textfield',
          '#title' => t('TinyPass Private Key (Sandbox)'),
          '#size' => 40,
          '#maxlength' => 40,
          '#default_value' => variable_get('tinypass_secret_key_sand', 'jeZC9ykDfvW6rXR8ZuO3EOkg9HaKFr90ERgEb3RW'),
          '#description' => t('TinyPass Application ID that identifies your client application.  Retreived from your TinyPass merchant account'),
  );


  $form['general']['tinypass_aid_prod'] = array(
          '#type' => 'textfield',
          '#title' => t('Application ID (Live)'),
          '#size' => 10,
          '#maxlength' => 10,
          '#default_value' => variable_get('tinypass_aid_prod', 'AIDAIDAID1'),
          '#description' => t('TinyPass Application ID that identifies your client application.  Retreived from your TinyPass merchant account'),
  );

  $form['general']['tinypass_secret_key_prod'] = array(
          '#type' => 'textfield',
          '#title' => t('Private Key (Live)'),
          '#size' => 40,
          '#maxlength' => 40,
          '#default_value' => variable_get('tinypass_secret_key_prod', 'Get From http://www.tinypass.com'),
          '#description' => t('TinyPass Application ID that identifies your client application.  Retreived from your TinyPass merchant account'),
  );


  $form['general']['tinypass_env'] = array(
          '#type' => 'radios',
          '#title' => t('Environment'),
          '#default_value' => variable_get('tinypass_env', 1),
          '#options' => array(t('Sandbox'), t("Live")),
          '#description' => t('Testing?  Use the sandbox environment'),
  );

  $form['general']['tinypass_access_message'] = array(
          '#type' => 'textarea',
          '#title' => t('Access Message'),
          '#default_value' => variable_get('tinypass_access_message', t("To continue, please purchase using TinyPass")),
          '#description' => t('Message to display when content access is denied'),
  );

  $form['general']['tinypass_access_message'] = array(
          '#type' => 'textarea',
          '#title' => t('Access Message'),
          '#default_value' => variable_get('tinypass_access_message', t("To continue, please purchase using TinyPass")),
          '#description' => t('Message to display when content access is denied'),
  );

  $form['general']['help'] = array(
          '#type' => 'markup',
          '#value' => '<p>Access Message can be fully customized by defining a <b>tinypass_denied_message.tpl.php</b> template</p>',
  );


  return system_settings_form($form);
}


/**
 * Validate pathauto_admin_settings form submissions.
 */
/**
 function tinypass_admin_settings_validate($form, &$form_state) {
 module_load_include('inc', 'pathauto');

 // Perform a basic check for HTML characters in the strings to remove field.
 if (strip_tags($form_state['values']['pathauto_ignore_words']) != $form_state['values']['pathauto_ignore_words']) {
 form_set_error('pathauto_ignore_words', t('The <em>Strings to remove</em> field must not contain HTML. Make sure to disable any WYSIWYG editors for this field.'));
 }

 // Validate that the separator is not set to be removed per http://drupal.org/node/184119
 // This isn't really all that bad so warn, but still allow them to save the value.
 $separator = $form_state['values']['pathauto_separator'];
 $punctuation = pathauto_punctuation_chars();
 foreach ($punctuation as $name => $details) {
 if ($details['value'] == $separator) {
 $action = $form_state['values']['pathauto_punctuation_' . $name];
 if ($action == 0) {
 drupal_set_message(t('You have configured the @name to be the separator and to be removed when encountered in strings. This can cause problems with your patterns and especially with the catpath and termpath patterns. You should probably set the action for @name to be "replace by separator"', array('@name' => $details['name'])), 'error');
 }
 }
 }
 }
 */
