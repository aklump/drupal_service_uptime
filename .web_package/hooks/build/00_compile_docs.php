<?php

/**
 * @file
 * Generates documentation, adjusts paths and adds to SCM.
 */

namespace AKlump\WebPackage;

$build
  ->generateDocumentation(FALSE)
  ->addFilesToScm([
    'README.md',
    'README.txt',
    'CHANGELOG.md',
  ])
  ->displayMessages();
