<?php
/**
 * @file
 * This file contains the admin and GUI callbacks for the module that are not needed in day-to-day operations
 */

/*
 * Implement callback for form construction
 */
function exact_target_api_configure_form($form_state) {
  $form = array();
  $cfg = _exact_target_api_get_config();

  $form[ET_CFG_ENDPOINT] = array(
    '#type' => 'select',
    '#title' => t('ExactTarget API endpoint'),
    '#description' => t('The URL that acts as the endpoint for the XML API calls, described <a href="http://wiki.memberlandingpages.com/030_Developer_Documentation/040_XML_API/ExactTarget_XML_API_Technical_Reference" target="_blank">here</a>.  If you are not sure what your endpoint type is, refer to <a href="http://wiki.memberlandingpages.com/The_Getting_Started_Guide/Set_Up_Your_Account" target="_blank">this page</a>.'),
    '#options' => array(
      ET_XML_ENDPOINT_NORMAL => t('Normal Endpoint'),
      ET_XML_ENDPOINT_S4 => t('S4 Instance Endpoint'),
    ),
    '#default_value' => (!empty($cfg[ET_CFG_ENDPOINT])) ? $cfg[ET_CFG_ENDPOINT] : ET_XML_ENDPOINT_NORMAL,
  );
  $form[ET_CFG_API_USER] = array(
    '#type' => 'textfield',
    '#title' => t('ExactTarget API Username'),
    '#default_value' =>  (!empty($cfg[ET_CFG_API_USER])) ? $cfg[ET_CFG_API_USER] : '',
    '#required' => TRUE,
  );
  $form[ET_CFG_API_PASS] = array(
    '#type' => 'password',
    '#title' => t('ExactTarget API Password'),
    '#required' => TRUE,
  );
  /* Not sure if we need to add this at this time...
  $form[ET_CFG_CHANNEL_MEMBER_ID] = array(
    '#type' => 'textfield',
    '#title' => t('ExactTarget Channel Member ID'),
    '#default_value' =>  (!empty($cfg[ET_CFG_CHANNEL_MEMBER_ID])) ? $cfg[ET_CFG_CHANNEL_MEMBER_ID] : '',
    '#required' => FALSE,
  );
  */
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save')
  );
  
  return $form;
}

/*
 * Implement callback for validation of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_validate($form, &$form_state) {
  
}

/*
 * Implement callback for submit handling of form 'exact_target_api_configure_form'
 */
function exact_target_api_configure_form_submit($form, &$form_state) {
  _exact_target_api_set_config($form_state['values']);
  drupal_set_message(t('ExactTarget API settings successfully saved.'), 'status');
}
