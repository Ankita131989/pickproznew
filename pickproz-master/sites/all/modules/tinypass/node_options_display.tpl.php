<?php
/**
 * @file
 * TinyPass node options form
 */
/**
 * Tinypass
 * ------------------------------------------------------------------------
 * @author    TinyPass, Inc
 * @copyright Copyright (C) 2011 tinypass.com. All Rights Reserved.
 * @license - http://www.gnu.org/licenses/lgpl-3.0.html GNU/LGPL
 * Websites: http://tinypass.com
 * Technical Support: http://developer.tinypass.com
 */
?>

<style>
  #tp_node_options .form-item {
    margin-top:0px;
    margin-bottom:0px;
  }
</style>
<?php
echo drupal_render($form['tp_enable_for_node']);

?>

<div id="tp_node_options">
	<?php echo drupal_render($form['tp_resource_id']) ?>
	<?php echo drupal_render($form['tp_resource_name']) ?>
	<?php echo drupal_render($form['tp_meta_id']) ?>
  <table class="tinypass_price_options">
    <tr>
      <th><?php echo t('Pricing Options');?>
				-
				<a id="tp_add_price_option" style="display:inline" onclick="tinypass.addPriceOption();return false;" href="#">Add</a>
				/
				<a id="tp_remove_price_option" style="display:inline" onclick="tinypass.removePriceOption();return false;" href="#">Remove</a>
				- <?php echo t("(between 1 and 3)") ?>
			</th>
    </tr>
		<?php for ($i = 0; $i < 3; $i++): ?>
    <tr>
				<?php
				$style = '';
				if ($i > 0) {
					$style = 'display:none';
					$enabled = $form['tp_option_enabled' . $i]['#value'];
					if ($enabled)
						$style = 'display:block';
				}
				?>
      <td >
        <div id="tp-options<?php echo $i?>" style="<?php echo $style?>" class="tp-price-option-form">

						<?php echo drupal_render($form['tp_option_enabled' . $i]) ?>
          <div>
            <table class="tp_add_tag_table">
              <tr>
                <td>
										<?php echo drupal_render($form['tp_price' . $i]) ?>
                </td>
                <td>
                  <table class="no-border" style="margin:0px">
                    <tr>
                      <td colspan="2" style="padding:0px" class="form-item"><label><?php echo t("Access Period:")?></label></td>
                    </tr>
                    <tr>
                      <td style="padding:0px" width=50>
													<?php echo drupal_render($form['tp_access_period'. $i]); ?>
                      </td>
                      <td style="padding:0px">
													<?php echo drupal_render($form['tp_access_period_type'. $i]); ?>
                      </td>
                    </tr>
                  </table>
                </td>
                <td>
										<?php echo drupal_render($form['tp_caption' . $i]) ?>
                </td>
              </tr>
              <tr>
                <td align="right">
                  <a href="#" onclick="tinypass.clearTimes(<?php echo $i ?>); return false;"><?php echo t("clear");?></a>
                </td>
                <td colspan="2">
                  <div style="float:left;">
											<?php echo drupal_render($form['tp_start_time' . $i]) ?>
                  </div>
                  <div style="float:left;">
											<?php echo drupal_render($form['tp_end_time' . $i]) ?>
                  </div>
                </td>
              </tr>
            </table>
          </div>
        </div>
      </td>
    </tr>
		<?php endfor; ?>
  </table>
</div>