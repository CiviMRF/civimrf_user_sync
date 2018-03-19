<?php

function roparun_user_sync_admin_form($form, &$form_state) {
	$profiles = cmrf_core_list_profiles();
  $profiles_options = array('' => t(' - Default profile -'));
  foreach($profiles as $profile_name => $profile) {
    $profiles_options[$profile_name] = $profile['label'];
  }
	
	$settings = roparun_user_sync_get_settings();
	
	$form['profile']['#type'] = 'select';
  $form['profile']['#title'] = t('CiviMRF Connection profile');
  $form['profile']['#options'] = $profiles_options;
  $form['profile']['#default_value'] = $settings['profile'];
	
	$form['cache']['#type'] = 'textfield';
  $form['cache']['#title'] = t('Cache timeout');
	$form['cache']['#required'] = true;
  $form['cache']['#default_value'] = $settings['cache'];
	
	$form['api_entity']['#type'] = 'textfield';
  $form['api_entity']['#title'] = t('CiviCRM Api Entity');
	$form['api_entity']['#required'] = true;
  $form['api_entity']['#default_value'] = $settings['api_entity'];
	
	$form['api_action']['#type'] = 'textfield';
  $form['api_action']['#title'] = t('CiviCRM Api Action');
	$form['api_action']['#required'] = true;
  $form['api_action']['#default_value'] = $settings['api_action'];
	
	$form['uid_attribute']['#type'] = 'textfield';
  $form['uid_attribute']['#title'] = t('UID Attribute');
	$form['uid_attribute']['#description'] = t('The UID is an unique user id. The attribute is the field in the results of the API call and is stored as a unique identifier at the user record.');
	$form['uid_attribute']['#required'] = true;
  $form['uid_attribute']['#default_value'] = $settings['uid_attribute'];
	
	$form['username_attribute']['#type'] = 'textfield';
  $form['username_attribute']['#title'] = t('Username Attribute');
	$form['username_attribute']['#description'] = t('The attribute is the field in the results of the API call and is stored as the username at the user record.');
	$form['username_attribute']['#required'] = true;
  $form['username_attribute']['#default_value'] = $settings['username_attribute'];
	
	$form['email_attribute']['#type'] = 'textfield';
  $form['email_attribute']['#title'] = t('Email Attribute');
	$form['email_attribute']['#description'] = t('The attribute is the field in the results of the API call and is stored as the email at the user record.');
	$form['email_attribute']['#required'] = true;
  $form['email_attribute']['#default_value'] = $settings['email_attribute'];
	
	$form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }

  $form['#submit'][] = 'roparun_user_sync_admin_form_submit';
  // By default, render the form using theme_system_settings_form().
  $form['#theme'] = 'system_settings_form';
	
	return $form;
}

/**
 * Submit handler for adding a new image style.
 */
function roparun_user_sync_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $profile = $form_state['values']['profile'];
   
  if ($profile) {
    variable_set('roparun_user_sync_profile', $profile);
  } else {
  	variable_del('roparun_user_sync_profile');
	}

	variable_set('roparun_user_sync_cache', $form_state['values']['cache']);
	variable_set('roparun_user_sync_api_entity', $form_state['values']['api_entity']);
	variable_set('roparun_user_sync_api_action', $form_state['values']['api_action']);
	variable_set('roparun_user_sync_uid_attribute', $form_state['values']['uid_attribute']);
	variable_set('roparun_user_sync_username_attribute', $form_state['values']['username_attribute']);
	variable_set('roparun_user_sync_email_attribute', $form_state['values']['email_attribute']);

	drupal_set_message(t('Settings have been saved'));
  $form_state['redirect'] = 'admin/config/civimrf/roparun_user_sync';
}
