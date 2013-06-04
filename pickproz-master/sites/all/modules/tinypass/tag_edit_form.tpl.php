<?php
/**
 * @file
 * TinyPass options form for taxonomy terms
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
<?php echo drupal_render($form['tinypass']['tp_enable_for_tag']);
$style = "" ?>
<div id="tinypass_meta_options">
	<?php echo drupal_render($form['tinypass']['term']) ?>
	<?php echo drupal_render($form['tinypass']['tp_resource_name']) ?>

  <br/> <hr> <br/>
  <div style="float:left">
    <strong><?php echo t('Metered access:');?> &nbsp;</strong>
  </div>
	<?php echo drupal_render($form['tinypass']['metered']) ?>
  <div class="clear"></div>

  <br/>
  <div id="tp-metered-count" class="tp-metered-options <?php echo $form['#vars']['tp_metered_count_class'] ?> ">
    <table class="tp-table small-table">
      <tr>
        <td width="150"><strong><?php echo t('Max views') ?></strong></td>
        <td><strong><?php echo t('Lockout Period') ?></strong></td>
      </tr>
      <tr>
        <td>
					<?php echo drupal_render($form['tinypass']['m_maa']) ?>
        </td>
        <td>
					<?php echo drupal_render($form['tinypass']['m_lp']) ?>
					<?php echo drupal_render($form['tinypass']['m_lp_type']) ?>
        </td>
      </tr>
    </table>

  </div>

  <div id="tp-metered-time" class="tp-metered-options <?php echo $form['#vars']['tp_metered_time_class'] ?> ">
    <table class="tp-table small-table">
      <tr>
        <td width="150"><strong><?php echo t('Trial Period') ?></strong></td>
        <td><strong><?php echo t('Lockout Period') ?></strong></td>
      </tr>
      <tr>
        <td>
					<?php echo drupal_render($form['tinypass']['m_tp']) ?>
					<?php echo drupal_render($form['tinypass']['m_tp_type']) ?>
        </td>
        <td>
					<?php unset($form['tinypass']['m_lp']['#printed']) ?>
					<?php unset($form['tinypass']['m_lp_type']['#printed']) ?>
					<?php echo drupal_render($form['tinypass']['m_lp']) ?>
					<?php echo drupal_render($form['tinypass']['m_lp_type']) ?>
        </td>
      </tr>
    </table>
  </div>

  <br/> <hr> <br/>
  <table class="tp-table">
    <tr>
      <th>Price Options -
				<a id="tp_add_price_option" style="display:inline" onclick="tinypass.addPriceOption();return false;" href="#">Add</a>
				/
				<a id="tp_remove_price_option" style="display:inline" onclick="tinypass.removePriceOption();return false;" href="#">Remove</a>
				- <?php echo t("(between 1 and 3)") ?>
			</th>
    </tr>
		<?php for ($i = 0; $i < 3; $i++): ?>
			<?php if ($i > 0): ?>
				<?php
				$style = 'display:none';
				$enabled = $form['tinypass']['tp_option_enabled' . $i]['#value'];
				if ($enabled)
					$style = 'display:block';
				?>
			<?php endif; ?>
    <tr>
      <td>
        <div id="tp-options<?php echo $i?>" style="<?php echo $style ?>" class="tp-price-option-form">
					<div style="display:inline-block">

							<?php echo drupal_render($form['tinypass']['tp_option_enabled' . $i]) ?>

            <table>
              <tr>
                <td>
										<?php echo drupal_render($form['tinypass']['tp_price' . $i]) ?>
                </td>
                <td>
                  <table class="no-border" style="margin:0px">
                    <tr>
                      <td colspan="2" class="form-item"><label><?php echo t("Access Period:")?></label></td>
                    </tr>
                    <tr>
                      <td style="padding:0px">
													<?php echo drupal_render($form['tinypass']['tp_access_period'. $i]); ?>
                      </td>
                      <td>
													<?php echo drupal_render($form['tinypass']['tp_access_period_type'. $i]); ?>
                      </td>
                    </tr>
                  </table>
                </td>
                <td>
										<?php echo drupal_render($form['tinypass']['tp_caption' . $i]) ?>
                </td>
              </tr>
							<tr>
								<td colspan="3">
									<div class="st">
										<div class="col">
												<?php echo drupal_render($form['tinypass']['tp_start_time' . $i]) ?>
										</div>
										<div class="col">
												<?php echo drupal_render($form['tinypass']['tp_end_time' . $i]) ?>
										</div>
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

	<?php echo drupal_render($form['submit']);?>
	<?php echo drupal_render($form['cancel']);?>
	<?php echo drupal_render($form['form_build_id']);?>
	<?php echo drupal_render($form['form_token']);?>
	<?php echo drupal_render($form['form_id']);?>
	<?php echo drupal_render($form['tinypass']['meta_id']);?>
</div>