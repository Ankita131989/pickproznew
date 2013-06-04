var tinypass = {

	showMeteredOptions: function(elem){
		var elem = jQuery(elem);
		var type = elem.val()

		jQuery(".tp-metered-options").hide();
		jQuery(".tp-metered-options :input").attr('disabled', 'disabled')
		jQuery("#tp-metered-" + type).show('medium');
		jQuery("#tp-metered-" + type + " :input").removeAttr('disabled');

	//this.log("Setting type:" + type);

	},

	tp_showOption: function(elem){
		var id = jQuery(elem).attr("id");
		var i = id.substring(id.length-1);
		jQuery('#tp_options' + i).toggle(jQuery(elem).attr('checked'));
	},


	clearTimes: function(i){
		jQuery("#edit-tp-start-time" + i + "-datepicker-popup-0").val("");
		jQuery("#edit-tp-start-time" + i + "-timeEntry-popup-1").val("");
		jQuery("#edit-tp-end-time" + i + "-datepicker-popup-0").val("");
		jQuery("#edit-tp-end-time" + i + "-timeEntry-popup-1").val("");
	},

	log:function(msg){
		if(console && console.log)
			console.log(msg);
	},

	addPriceOption: function(){
		var count = jQuery(".tp-price-option-form:visible").size();
		if(count <= 3){
			var opt = count;
			jQuery("#tp-options" + opt).show('fast');
			jQuery("#tp-options" + opt).find("input:hidden").val("1");
		}
	},
	removePriceOption: function(){
		var count = jQuery(".tp-price-option-form:visible").size();
		if(count > 1){
			var opt = count-1;
			jQuery("#tp-options" + opt).hide('fast');
			jQuery("#tp-options" + opt).find("input:hidden").val("0");
		}
	}
}

jQuery(function(){
	tinypass.showMeteredOptions(document.getElementsByName("metered")[0])
})
