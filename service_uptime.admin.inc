<?php

/**
 * @file
 * Administration page callbacks for the service_uptime module.
 */

/**
 * Form builder. Configure my_module.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function service_uptime_admin_settings() {
  $form = array();
  $form['#attached']['css'][] = array(
    'type' => 'file',
    'data' => drupal_get_path('module', 'service_uptime') . '/css/service_uptime.css',
  );
  $stats_link = variable_get('service_uptime_qs', NULL);

  if (!$stats_link) {
    $form['signup'] = array(
      '#markup' => t('<h2><a href="!url" target="_blank">Get your free account &rarr;</a></h2>', [
        '!url' => service_uptime_url(),
      ]),
    );
  }

  if ($hash = service_uptime_get_public_hash()) {
    $form['edit_service'] = array(
      '#type' => 'fieldset',
      '#title' => t('Monitor Settings'),
      '#description' => t('Use these values when adding your "website" monitor to your Service Uptime account.'),
      '#weight' => -10,
      '#collapsible' => TRUE,
      '#collapsed' => !empty($stats_link),
    );

    $form['edit_service']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#value' => variable_get('site_name'),
      '#attributes' => array('readonly' => TRUE),
    );

    $form['edit_service']['service'] = array(
      '#type' => 'textfield',
      '#title' => t('Service'),
      '#value' => 'web page',
      '#attributes' => array('readonly' => TRUE),
    );

    $form['edit_service']['page_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Page Url'),
      '#value' => ($url = url(SERVICE_UPTIME_PATH_CHECK, array(
        'absolute' => TRUE,
        'query' => array('s' => variable_get('service_uptime_private_key', NULL)),
      ))),
      '#attributes' => array('readonly' => TRUE),
      '#field_suffix' => l(t('Test &rarr;'), $url, array(
        'html' => TRUE,
        'attributes' => array('target' => '_blank'),
      )),
    );

    $form['edit_service']['service_uptime_public_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Search string'),
      '#description' => t('Page should contain this text'),
      '#default_value' => $hash,
      '#size' => 60,
      '#attributes' => array('readonly' => TRUE),
    );
  }

  $form['service_uptime_qs'] = array(
    '#type' => 'textfield',
    '#description' => t('<p>Click <a href="!url" target="_blank">here</a> and locate the "Direct Link" URL; copy it in this box.  It will resemble <strong>!this</strong></p><p>You also will have to <a href="!public" target="_blank">enable</a> public statistics.</p>', array(
      '!url' => service_uptime_url('/users/openstat.php'),
      '!this' => SERVICE_UPTIME_EXAMPLE_LINK,
      '!public' => service_uptime_url('/users/openstatmulti.php'),
    )),
    '#title' => t('Service Uptime Statistics Link'),
    '#default_value' => $stats_link,
    '#required' => FALSE,
    '#size' => 120,
  );

  $form['dev'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['dev']['service_uptime_force_failure'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force a failure on next check'),
    '#description' => t('(Use sparingly as it will skew your results; do this to check if the failure email is working correctly.)'),
    '#default_value' => variable_get('service_uptime_force_failure', NULL),
  );

  $form['#validate'][] = 'service_uptime_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for service_uptime_admin_settings_validate().
 */
function service_uptime_admin_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['service_uptime_qs']) && !preg_match(SERVICE_UPTIME_REGEX, $form_state['values']['service_uptime_qs'])) {
    form_set_error('service_uptime_qs', t('The format of your link is incorrect.  It should look like this: <strong>@example</strong>', array(
      '@example' => SERVICE_UPTIME_EXAMPLE_LINK,
    )));
  }
}
