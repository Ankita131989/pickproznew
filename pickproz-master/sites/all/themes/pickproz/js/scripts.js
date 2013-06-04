jQuery(document).ready(function($){					
	 
	$('input#edit-submitted-first-name').focus(function(){
		var txtval = $('input#edit-submitted-first-name').val();
		if(txtval == 'First name'){ $(this).val('');}
	});
	
	$('input#edit-submitted-first-name').focusout(function(){
		 
		var txtval = $('input#edit-submitted-first-name').val();
		if(txtval == ""){ $('input#edit-submitted-first-name').val('First name');}

	});
	
	$('input#edit-submitted-last-name').focus(function(){
		var txtval = $('input#edit-submitted-last-name').val();
		if(txtval == 'Last name'){ $(this).val('');}
	});
	
	$('input#edit-submitted-last-name').focusout(function(){
		 
		var txtval = $('input#edit-submitted-last-name').val();
		if(txtval == ""){ $('input#edit-submitted-last-name').val('Last name');}

	});
	
	$('input#edit-submitted-email-address').focus(function(){
		var txtval = $('input#edit-submitted-email-address').val();
		if(txtval == 'Email address'){ $(this).val('');}
	});
	
	$('input#edit-submitted-email-address').focusout(function(){
		 
		var txtval = $('input#edit-submitted-email-address').val();
		if(txtval == ""){ $('input#edit-submitted-email-address').val('Email address');}

	});
});