<?php
/**
 * @file
 * This file contains the admin and GUI callbacks for the module that are not needed in day-to-day operations
 */

/**
 * Implements callback for form construction
 */
function exact_target_api_configure_form($form, &$form_state) {
  $form = array();
  $cfg = _exact_target_api_get_config();

  $form['et_cfg_endpoint'] = array(
    '#type' => 'select',
    '#title' => t('App Center Authentication Target'),
     '#description' => t('Login to <a href="https://appcenter.s1.marketingcloudapps.com">App Center</a>, select your app and in the "Authentication and Integration" section you will see the "Authentication Target"'),
    '#options' => array(
      NULL => t(' -- select an option -- '),
      's1'  => t('S1 Instance'),
      's4'  => t('S4 Instance'),
      's6'  => t('S6 Instance'),
      's7'  => t('S7 Instance'),
      'test'  => t('Production Support (test) Instance'),
    ),
    '#required' => TRUE,
    '#default_value' => (!empty($cfg['et_cfg_endpoint'])) ? $cfg['et_cfg_endpoint'] : 'test',
  );
  $form['et_api_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App Center Client ID'),
    '#description' => t('Login to <a href="https://appcenter.s1.marketingcloudapps.com">App Center</a>, select your app and in the "Authentication and Integration" section you will see the "Client ID"'),
    '#default_value' => (!empty($cfg['et_api_client_id'])) ? $cfg['et_api_client_id'] : '',
    '#required' => TRUE,
  );
  $form['et_api_client_secret'] = array(
    '#type' => 'password',
    '#title' => t('App Center Client Secret'),
    '#description' => t('Login to <a href="https://appcenter.s1.marketingcloudapps.com">App Center</a>, select your app and in the "Authentication and Integration" section you will see the "Client Secret"'),
    // this is not required because we don't want the user to have to enter in
    // the secret again is ever making other changes on this page
  );

  $form['exact_target_list_cache_ttl'] = array(
    '#type' => 'select',
    '#title' => t('List Cache Time To Live (TTL)'),
    '#description' => t('Upon submission of this form all available lists are requested from the Exact Target servers and the information is cached in the database. How old should we let these items become before they are cached again?'),
    '#default_value' => variable_get('exact_target_list_cache_ttl', 86400),
    '#options' => array(
      600 => t('10 minutes'),
      3600 => t('1 hour'),
      86400 => t('1 day'),
      604800 => t('1 week'),
    ),
  );
  /* Not sure if we need to add this at this time...
   $form['et_channel_member_id'] = array(
   '#type' => 'textfield',
   '#title' => t('ExactTarget Channel Member ID'),
   '#default_value' =>  (!empty($cfg['et_channel_member_id'])) ? $cfg['et_channel_member_id'] : '',
   '#required' => FALSE,
   );
  */

  $form['exact_target_api_wsdl_cache_ttl'] = array(
    '#type' => 'select',
    '#title' => t('WSDL Cache Time To Live (TTL)'),
    '#description' => t('The Web Service Definition Language (WSDL) file will be downloaded and cached locally to reduce network calls and sped up requests. How long do you want this cache to live?'),
    '#default_value' => variable_get('exact_target_api_wsdl_cache_ttl', 604800),
    '#options' => array(
      0 => t('Don\'t cache'),
      600 => t('10 minutes'),
      3600 => t('1 hour'),
      86400 => t('1 day'),
      604800 => t('1 week'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements callback for validation of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_validate($form, &$form_state) {
  // Go ahead and store the values as the test API call won't work otherwise: 
  _exact_target_api_set_config($form_state['values']);

  // Test the new creds using the values we were just given:
  $lists = exact_target_api_get_lists();
  if (empty($lists)) {
    // We should ALWAYS get something back as the "global" list is always present
    form_set_error('', t('Failed to retrieve subscription lists in test call to the ExactTarget server.  Please check your credentials and try again.'));
  }
  else {
    $form_state['storage']['et_lists'] = $lists;
  }
}

/**
 * Implements callback for submit handling of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_submit($form, &$form_state) {
  drupal_set_message(t('ExactTarget API settings successfully saved.'), 'status');

  // Now initialize/refresh the subscription list cache so it doesn't bog down any future API calls
  $batch = array(
    'operations' => array(
      array('_exact_target_process_lists', array($form_state['storage']['et_lists'])),
    ),
    'finished' => '_exact_target_process_lists_finished',
    'title' => t('Processing initial ExactTarget subscription lists'),
    'init_message' => t('Batch is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Batch processing of ExactTarget subscription lists has encountered an error.'),
    'file' => drupal_get_path('module', 'exact_target_api') . '/exact_target_api.admin.inc',
  );
  batch_set($batch);
}

/**
 * Implements callback for batch processing run
 *
 * Bits and pieces shamelessly copied from http://drupal.org/node/180528
 */
function _exact_target_process_lists($lists, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($lists);
  }

  // Retrieve the next list's information and cache it

  $list = array_slice($lists,$context['sandbox']['progress'],1)[0];
  $listinfo = exact_target_api_get_list_info($list);

  // Update our progress information.

  $context['sandbox']['progress']++;
  $context['results'][] = t('Processed ExactTarget list ID %id', array('%id' => $listid));
  $context['message'] = t('Now processing list %count of %max', array('%count' => $context['sandbox']['progress'], '%max' => $context['sandbox']['max']));

  // Inform the batch engine that we are not finished,

  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Implements callback for completion of batch processing run
 *
 * Shamelessly copied from http://drupal.org/node/180528
 */
function _exact_target_process_lists_finished($success, $results, $operations) {
  if ($success) {
    $message = count($results) . ' ExactTarget subscription lists processed.';
  }
  else {
    // An error occurred.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}

