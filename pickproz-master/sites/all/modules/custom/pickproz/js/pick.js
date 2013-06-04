jQuery.noConflict()(function($){
$('.form-item-field-will-they-win-by-points-und').hide();
        $('#edit-field-pick-winner-und').change(function(){
            var str  = $('option:selected').text();
            if(str.indexOf('-') > 0) {
               $('.form-item-field-will-they-win-by-points-und').show();
            } else {
                $('.form-item-field-will-they-win-by-points-und').hide();
                
            }
            var str  = $('option:selected').text();
            if(str.indexOf('+') > 0) {
               $('.form-item-field-will-they-win-by-points-und').hide();
            } else {
                $('.form-item-field-will-they-win-by-points-und').show();
                
            }
        });});



