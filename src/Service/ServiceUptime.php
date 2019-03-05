<?php

namespace Drupal\service_uptime\Service;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\PrivateKey;
use Drupal\Core\Url;

/**
 * Core functionality provided by the service_uptime module.
 */
class ServiceUptime {

  /**
   * Message for not-yet-configured.
   *
   * @var string
   */
  const NOT_CONFIGURED_MESSAGE = 'You have not yet configured your Service Uptime settings; please <a href=":admin_settings_url">click here</a>.';

  /**
   * The example format of a link.
   *
   * @var string
   */
  const EXAMPLE_LINK = 'https://www.serviceuptime.com/users/uptimemonitoring.php?S=a07db1b0170f92d081fec95806daef7f&Id=12259';

  /**
   * This is the regex for checking user token links.
   *
   * @var string
   */
  const REGEX = '/^https?:\/\/www\.serviceuptime.com\/users\/(?:uptime)?monitoring\.php(\?(S=\w+)\&(Id=\d+))$/';

  /**
   * An instance of the config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * The private key service.
   *
   * @var \Drupal\Core\PrivateKey
   */
  protected $privateKey;

  /**
   * The settings for this module.
   *
   * @var \Drupal\Core\Config\ImmutableConfig
   */
  protected $settings;

  /**
   * ServiceUptime constructor.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $config_factory
   *   An instance of ConfigFactory.
   */
  public function __construct(ConfigFactoryInterface $config_factory, PrivateKey $private_key) {
    $this->privateKey = $private_key;
    $this->configFactory = $config_factory;
    $this->settings = $this->configFactory->get('service_uptime.settings');
  }

  /**
   * Create an absolute URL to service uptime (with affiliate link).
   *
   * @param string $path
   *   The path at service uptime.
   *
   * @return string
   *   The full URL with affiliate link when appropriate.
   */
  public function getUrl($path = '/') {
    $options = [];
    if ($path === '/') {
      $options['query']['aff'] = 'R779';
    }

    return Url::fromUri('http://www.serviceuptime.com/' . trim($path, '/'), $options)
      ->toString();
  }

  /**
   * Get the hash (Search String)
   *
   * @param null $seed
   *   A seed to use instead of the default.
   *
   * @return false|string
   *   FALSE if the hash cannot be generated. String otherwise.
   */
  public function getSearchString($seed = NULL) {
    $seed = empty($seed) ? $this->settings->get('public_seed') : $seed;
    return $seed ? md5($seed . $this->privateKey->get()) : FALSE;
  }

  /**
   * Return the service uptime query string for this website.
   *
   * This is a portion of the Service Uptime Statistics Link entered in
   * settings.
   */
  public function getAccountQueryString() {
    if (($link = $this->settings->get('stats_url'))
      && preg_match(ServiceUptime::REGEX, $link, $found) && $found[1]) {
      return $found[1];
    };

    return FALSE;
  }

  /**
   * Return the statistics markup from Service Uptime for an URL.
   *
   * @return string
   *   The markup from Service Uptime.
   *
   * @throws \RuntimeException
   *   If the server cannot make a remote connection.
   */
  public function getStatisticsMarkup() {
    $url = $this->settings->get('stats_url');
    if (intval(get_cfg_var('allow_url_fopen')) && function_exists('file_get_contents')) {
      if (!($content = @file_get_contents($url))) {
        return FALSE;
      }
    }
    elseif (intval(get_cfg_var('allow_url_fopen')) && function_exists('file')) {
      if (!($content = @file($url))) {
        return FALSE;
      }

      $content = @implode('', $content);
    }
    elseif (function_exists('curl_init')) {
      $ch = curl_init($url);
      curl_setopt($ch, CURLOPT_HEADER, 0);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      ob_start();
      $content = curl_exec($ch);
      if (curl_error($ch)) {
        $content = FALSE;
      }
      curl_close($ch);
      if (!$content) {
        return $content;
      }
    }
    if ($content) {
      if (!preg_match('/<section\s*id="content".+?<\/section>/si', $content, $matches)) {
        return FALSE;
      }

      return $matches[0];
    }
    throw new \RuntimeException("This server is unable to make a remote connection.");
  }

}
