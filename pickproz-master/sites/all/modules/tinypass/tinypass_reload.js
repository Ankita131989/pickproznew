 /**
 * Tinypass 
 * ------------------------------------------------------------------------
 * @author    TinyPass, Inc
 * @copyright Copyright (C) 2011 tinypass.com. All Rights Reserved.
 * @license - http://www.gnu.org/licenses/lgpl-3.0.html GNU/LGPL
 * Websites: http://tinypass.com
 * Technical Support: http://developer.tinypass.com
 */
if(typeof tinypass_reloader != 'function') {
	function tinypass_reloader(status){
		if(status.state == 'granted'){
			window.location.reload();
		}
	}
}