<?php

$plugin = array(
  'title' => t('Display (view mode) extraction'),
  'selector' => '[x-drupal-type=display]',
  'callback' => '_hyde_display_extraction',
);

function _hyde_display_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("Display (view mode) extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $regions = array();
  foreach ($element->find('[x-drupal-region]') as $el) {
    $el_name = $el->attr('x-drupal-name');
    $el_label = $el->attr('x-drupal-label');
    if (empty($el_label)) {
      $el_label = $el_name;
    }
    // TODO: is there some other way to prevent var name clash?
    $regions['ds_' . $el_name] = $el_label;
    $el->removeAttr('x-drupal-region')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');
    if ($el->hasAttr('x-drupal-no-wrap')) {
      $el->before('{{[[PHP_OPEN print $ds_' . $el_name . '; PHP_CLOSE]]}}');
      $el->remove();
    }
    else {
      $el->html('{{[[PHP_OPEN print $ds_' . $el_name . '; PHP_CLOSE]]}}');
    }
  }

  if ($element->hasAttr('x-drupal-no-wrap')) {
    $result = hyde_replace_php_tags($element->innerHTML());
  }
  else {
    $result = hyde_replace_php_tags($element->html());
  }
  $theme->view_modes[$name] = array(
    'label' => $label,
    'template' => $result,
    'regions' => $regions,
  );

}

