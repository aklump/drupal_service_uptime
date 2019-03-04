<?php
/**
 * @file
 * Administration page callbacks for the service_uptime module.
 *
 * @ingroup service_uptime
 * @{
 */

/**
 * Form builder. Configure my_module.
 *
 * @ingroup forms
 * @see system_settings_form().
 */

function service_uptime_admin_settings() {
  $form = array();
  $form['#attached']['css'][] = array(
    'type' => 'file',
    'data' => drupal_get_path('module', 'service_uptime') . '/css/service_uptime.css',
  );
  $qs = variable_get('service_uptime_qs', NULL);

  if (!$qs) {
    $form['singup'] = array(
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
      '#markup' => _service_uptime_l('Get your free account &rarr;'),
    );
  }

  if ($hash = _service_uptime_hash()) {

    $form['edit_service'] = array(
      '#type' => 'fieldset',
      '#title' => t('Monitor Settings'),
      '#description' => t('Use these values when adding your "website" monitor to your Service Uptime account.'),
      '#weight' => -10,
      '#collapsible' => TRUE,
      '#collapsed' => !empty($qs),
    );

    $form['edit_service']['new_service'] = array(
      '#type' => 'markup',
      '#value' => '<div>' . t('!link a new <em>web page</em> service, using the following settings:', array(
          '!link' => _service_uptime_l('Add', '/users/services-edit.php'),
        )) . '</div>',
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
        'query' => array('s' => _service_uptime_private_key()),
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
    '#description' => t('<p>Enter your statistics link.  Click !link and find the "Direct Link".  Copy it here. It will resemble <strong>!this</strong>.</p><p>You also will have to !public public statistics.</p>', array(
      '!link' => _service_uptime_l('this page', '/users/openstat.php'),
      '!public' => _service_uptime_l('enable', '/users/openstatmulti.php'),
      '!this' => SERVICE_UPTIME_EXAMPLE_LINK,
    )),
    '#title' => t('Service Uptime Statistics Link'),
    '#default_value' => $qs,
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

  $form['dev']['service_uptime_stock_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Embed the Service Uptime stylesheet on your Drupal statistics page?'),
    '#default_value' => variable_get('service_uptime_stock_css', FALSE),
  );

  $form['#validate'][] = 'service_uptime_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for service_uptime_admin_settings_validate().
 */
function service_uptime_admin_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['service_uptime_qs']) && !preg_match(SERVICE_UPTIME_REGEX, $form_state['values']['service_uptime_qs'])) {
    form_set_error('service_uptime_qs', t('The format of your token is incorrect.  It should look like this: <strong>@example</strong>', array(
      '@example' => SERVICE_UPTIME_EXAMPLE_LINK,
    )));
  }
}

/** @} */ //end of group service_uptime
