<?php

$plugin = array(
  'title' => t('List style extraction'),
  'selector' => '[x-drupal-type=list]',
  'callback' => '_hyde_list_style_extraction',
);

function _hyde_list_style_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("List style extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $parts = array(
    'x-drupal-list-title' => '$title',
    'x-drupal-list-header' => '$header',
    'x-drupal-list-inner' => '$content',
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
  $theme->list_styles[$name] = array(
    'label' => $label,
    'name' => $name,
    'template' => $result,
  );

}

