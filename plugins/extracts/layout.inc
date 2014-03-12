<?php

$plugin = array(
  'title' => t('Layout extraction'),
  'selector' => '[x-drupal-type=layout]',
  'callback' => '_hyde_layout_extraction',
);


function _hyde_layout_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("Layout extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $regions = array();
  $sub_regions = array();

  foreach ($element->find('[x-drupal-region]') as $el) {
    if ($el->hasAttr('x-drupal-name')) {
      // Sub regions - for removal
      // TODO: double check login in this sub-region removal
      //dpm("parent: " . $el->attr('x-drupal-name'));
      foreach ($el->find('[x-drupal-region]') as $sub_el) {
        //dpm("child: " . $sub_el->attr('x-drupal-name'));
        $sub_regions[] = $sub_el->attr('x-drupal-name');
      }

      $el_name = $el->attr('x-drupal-name');

      if (!in_array($el_name, $sub_regions)) {
        $el_label = $el->attr('x-drupal-label');
        if (empty($el_label)) {
          $el_label = $el_name;
        }
        // TODO: is there some other way to prevent var name clash?
        $regions[$el_name] = $el_label;
        $el->removeAttr('x-drupal-region')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');
        if ($el->hasAttr('x-drupal-no-wrap')) {
          $el->before("{{[[PHP_OPEN print \$content['" . $el_name . "']; PHP_CLOSE]]}}");
          $el->remove();
        }
        else {
          $el->html("{{[[PHP_OPEN print \$content['" . $el_name . "']; PHP_CLOSE]]}}");
        }
      }
    }

  }
  /*
  foreach ($sub_regions as $sub) {
    unset($regions[$sub]);
  }
  */

  if ($element->hasAttr('x-drupal-no-wrap')) {
    $result = hyde_replace_php_tags($element->innerHTML());
  }
  else {
    $result = hyde_replace_php_tags($element->html());
  }
  $theme->layouts[$name] = array(
    'label' => $label,
    'template' => $result,
    'regions' => $regions,
  );

}
