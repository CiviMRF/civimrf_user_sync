<?php

function civimrf_user_sync_admin_form($form, &$form_state) {
	$profiles = cmrf_core_list_profiles();
  $profiles_options = array('' => t(' - Default profile -'));
  foreach($profiles as $profile_name => $profile) {
    $profiles_options[$profile_name] = $profile['label'];
  }
	
	$settings = civimrf_user_sync_get_settings();
	
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
	
	$form['api_count_action']['#type'] = 'textfield';
  $form['api_count_action']['#title'] = t('CiviCRM Api Count Action');
	$form['api_count_action']['#required'] = true;
  $form['api_count_action']['#default_value'] = $settings['api_count_action'];
	
	$form['api_parameters']['#type'] = 'textarea';
  $form['api_parameters']['#title'] = t('CiviCRM Api Parameters');
	$form['api_parameters']['#description'] = t('Enter the api parameters in JSON format. E.g. {"contact_sub_type": "Student", "is_deleted": "0", "is_deceased": "0"}');
	$form['api_parameters']['#required'] = true;
  $form['api_parameters']['#default_value'] = $settings['api_parameters'];
	
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
  
  $form['hide_username_field']['#type'] = 'checkbox';
  $form['hide_username_field']['#title'] = t('Hide username field');
  $form['hide_username_field']['#description'] = t('Hides the username field from the account page');
  $form['hide_username_field']['#default_value'] = $settings['hide_username_field'];
  
  $form['hide_email_field']['#type'] = 'checkbox';
  $form['hide_email_field']['#title'] = t('Hide email field');
  $form['hide_email_field']['#description'] = t('Hides the e-mail field from the account page');
  $form['hide_email_field']['#default_value'] = $settings['hide_email_field'];
	
	$roles = array_map('check_plain', user_roles(TRUE));
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in Form API not supporting a single disabled
  // checkbox within a set of checkboxes.
  // @todo This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' =>  $settings['roles'],
    '#options' => $roles,
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );
	
	$form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }

  $form['#submit'][] = 'civimrf_user_sync_admin_form_submit';
  // By default, render the form using theme_system_settings_form().
  $form['#theme'] = 'system_settings_form';
	
	return $form;
}

/**
 * Submit handler for adding a new image style.
 */
function civimrf_user_sync_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $profile = $form_state['values']['profile'];
   
  if ($profile) {
    variable_set('civimrf_user_sync_profile', $profile);
  } else {
  	variable_del('civimrf_user_sync_profile');
	}

	variable_set('civimrf_user_sync_cache', $form_state['values']['cache']);
	variable_set('civimrf_user_sync_api_entity', $form_state['values']['api_entity']);
	variable_set('civimrf_user_sync_api_action', $form_state['values']['api_action']);
	variable_set('civimrf_user_sync_api_count_action', $form_state['values']['api_count_action']);
	variable_set('civimrf_user_sync_api_parameters', $form_state['values']['api_parameters']);
	variable_set('civimrf_user_sync_uid_attribute', $form_state['values']['uid_attribute']);
	variable_set('civimrf_user_sync_username_attribute', $form_state['values']['username_attribute']);
	variable_set('civimrf_user_sync_email_attribute', $form_state['values']['email_attribute']);
  variable_set('civimrf_hide_username_field', $form_state['values']['hide_username_field']);
  variable_set('civimrf_hide_email_field', $form_state['values']['hide_email_field']);
	variable_set('civimrf_user_sync_roles', $form_state['values']['roles']);

	drupal_set_message(t('Settings have been saved'));
  $form_state['redirect'] = 'admin/config/civimrf/civimrf_user_sync';
}
