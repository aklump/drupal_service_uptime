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

  drupal_add_css(drupal_get_path('module', 'service_uptime') . '/css/service_uptime.css');

  $form = array();

  $first_run = variable_get('service_uptime_first_run', FALSE);

  if ($first_run) {
    $form['signup'] = array(
      '#type' => 'markup',
      '#value' => '<div class="service-uptime-signup">' . t('!link1 to sign up for you Free or Paid Account at !link<sup>1</sup>', array(
          '!link1' => _service_uptime_l(t('Click here')),
          '!link' => _service_uptime_l(),
        )) . '</div><!-- /.service-uptime-signup --><div class="service-uptime-footnote"><sup>1</sup>' . t("This link contains this module's author's affiliate information for this service. <a href=\"@url\" onclick=\"window.open(this.href); return false;\">Click here</a> for more info.", array(
          '@url' => SERVICE_UPTIME_URL_AFFILIATE_EXPLANATION,
        )) . '</div><!-- /.service-uptime-footnote -->',
    );
  }

  //if (!$first_run && !variable_get('service_uptime_private_key', '')) {
  //  _service_uptime_create_private_key();
  //  drupal_set_message(t('Your Search String has been regenerated, please <strong>immediately update</strong> your link at !link as appropriate.', array(
  //    '!link' => _service_uptime_l(NULL, 'https://www.serviceuptime.com/users/services.php'),
  //  )), 'warning');
  //}
  //$form['service_uptime_private_key'] = array(
  //  '#type' => 'textfield',
  //  '#title' => t('Private Key'),
  //  '#default_value' => variable_get('service_uptime_private_key', ''),
  //  '#required' => TRUE,
  //  '#size' => 60,
  //  '#attributes' => array('readonly' => TRUE),
  //);

  if ($hash = _service_uptime_hash()) {

    $form['edit_service'] = array(
      '#type' => 'fieldset',
      '#title' => t('Service Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => !$first_run,
    );

    $form['edit_service']['new_service'] = array(
      '#type' => 'markup',
      '#value' => '<div>' . t('!link a new <em>web page</em> service, using the following settings:', array(
          '!link' => _service_uptime_l('Add', 'https://www.serviceuptime.com/users/services-edit.php'),
        )) . '</div>',
    );
    //$form['edit_service_new_service'] = array(
    //  '#markup' => t('Add a new webpage service using the following settings'),
    //);

    $form['edit_service']['page_url'] = array(
      '#type' => 'textfield',
      '#description' => t('When adding your service to Service Uptime, select <em>web page</em> and copy this value to <em>Page Url</em>'),
      '#title' => t('Page URL'),
      '#value' => url(SERVICE_UPTIME_PATH_CHECK, array(
        'absolute' => TRUE,
        'query' => array('s' => _service_uptime_private_key()),
      )),
      '#attributes' => array('readonly' => TRUE),
    );

    $form['edit_service']['service_uptime_public_key'] = array(
      '#type' => 'textfield',
      '#description' => t('When adding your service to Service Uptime, select <em>web page</em> and copy this value to <em>Search String</em>'),
      '#title' => t('Search String'),
      '#default_value' => $hash,
      '#size' => 60,
      '#attributes' => array('readonly' => TRUE),
    );

    $form['edit_service']['service_uptime_force_failure'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force a failure on next check (Use sparingly as it will skew your results; do this to check if the failure email is working correctly.)'),
      '#default_value' => variable_get('service_uptime_force_failure', NULL),
    );
  }

  $form['service_uptime_qs'] = array(
    '#type' => 'textfield',
    '#description' => t('Enter your statistics link.  This comes from your Service Uptime account from !link. It will look like <strong>!this</strong>.  You will have to look inside the Option 2 PHP option.  You also will have to enable public statistics.', array(
      '!link' => l(t('this page'), 'https://www.serviceuptime.com/users/openstat.php'),
      '!this' => SERVICE_UPTIME_EXAMPLE_LINK,
    )),
    '#title' => t('Service Uptime Statistics Link'),
    '#default_value' => variable_get('service_uptime_qs', NULL),
    '#required' => FALSE,
    '#size' => 120,
  );

  $form['service_uptime_stock_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Embed the Service Uptime stylesheet on your Drupal statistics page?'),
    '#default_value' => variable_get('service_uptime_stock_css', FALSE),
  );

  $form['#validate'][] = 'service_uptime_admin_settings_validate';
  $form['#submit'][] = 'service_uptime_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form validation handler for service_uptime_admin_settings_validate().
 */
function service_uptime_admin_settings_validate($form, &$form_state) {
  if (!preg_match(SERVICE_UPTIME_REGEX, $form_state['values']['service_uptime_qs'])) {
    form_set_error('service_uptime_qs', t('The format of your token is incorrect.  It should look like this: <strong>@example</strong>', array(
      '@example' => SERVICE_UPTIME_EXAMPLE_LINK,
    )));
  }
}

/**
 * Form submission handler
 */
function service_uptime_admin_settings_submit($form, &$form_state) {
  variable_del('service_uptime_first_run');
}

/** @} */ //end of group service_uptime
