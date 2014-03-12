<?php

$plugin = array(
  'title' => t('Menu style extraction'),
  'selector' => '[x-drupal-type=menu]',
  'callback' => '_hyde_menu_extraction',
);

function _hyde_menu_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("Menu style extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $wrapper = $element->attr('class');
  $active = $element->find('[x-drupal-menu-active]');
  if ($active->size()) {
    $active = $active->attr('class');
  }
  else {
    $active = 'active-class';
  }
  $item = $element->find('[x-drupal-menu-item]');
  if ($item->size()) {
    $item = $item->attr('class');
  }
  else {
    $item = '';
  }

  $theme->menu_styles[$name] = array(
    'label' => $label,
    'name' => $name,
    'wrapper' => $wrapper,
    'item' => $item,
    'active' => $active,
  );

}

