<?php

$plugin = array(
  'title' => t('Box style extraction'),
  'selector' => '[x-drupal-type=box]',
  'callback' => '_hyde_box_style_extraction',
);

function _hyde_box_style_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("Box style extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $parts = array(
    'x-drupal-box-title' => '$title',
    'x-drupal-box-inner' => '$content',
  );
  foreach ($parts as $part_attr => $part_var) {
    $part_element = $element->find("[{$part_attr}]");
    if ($part_element->size()) {
      if ($part_element->hasAttr('x-drupal-no-wrap')) {
        $part_element->before('{{[[PHP_OPEN print ' . $part_var . '; PHP_CLOSE]]}}');
        $part_element->remove();
      }
      else {
        $part_element->html('{{[[PHP_OPEN print ' . $part_var . '; PHP_CLOSE]]}}');
        $part_element->removeAttr($part_attr);
      }
    }
  }

  if ($element->hasAttr('x-drupal-no-wrap')) {
    $result = hyde_replace_php_tags($element->innerHTML());
  }
  else {
    $result = hyde_replace_php_tags($element->html());
  }
  $theme->box_styles[$name] = array(
    'label' => $label,
    'name' => $name,
    'template' => $result,
  );

}

