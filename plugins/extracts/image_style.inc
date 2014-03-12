<?php

$plugin = array(
  'title' => t('Image style extraction'),
  'selector' => '[x-drupal-type=image_style]',
  'callback' => '_hyde_image_style_extraction',
);

function _hyde_image_style_extraction($element, $theme) {

  $name = $element->attr('x-drupal-name');
  $label = $element->attr('x-drupal-label');
  if (empty($label)) {
    $label = $name;
  }
  drupal_set_message("Image style extraction for {$name}");
  $element->removeAttr('x-drupal-type')->removeAttr('x-drupal-name')->removeAttr('x-drupal-label');

  $w = $element->attr('width');
  $h = $element->attr('height');

  $theme->image_styles[$name] = array(
    'label' => $label,
    'effects' => array(
      array(
        'name' => 'image_scale',
        'data' => array(
          'width' => $w,
          'height' => $h,
          'upscale' => 1,
        ),
        'weight' => 0,
      ),
    ),
  );

}

