function displayWechallBox(data) {
    console.log("Displaying box with data");
    console.log(data);
    // create a div, draggable, with form inside
    var $div = $('<div />').appendTo('body');
    $div.attr('id', 'wechallform');

    $("#wechallform").hide();
    $("#wechallform").html(
    		"<p>Submit token</p>"+
    		"<form id=\"realwechallform\" action=\""+data["form"]+"\" enctype=\"application/x-www-form-urlencoded\" method=\"post\">"+
		"<input type=\"hidden\" name=\"wfid\" value=\""+data["wfid"]+"\" />"+
		"<input type=\"hidden\" name=\"password_solution\" value=\""+data["pass"]+"\"/>"+
		"<input type=\"hidden\" name=\"igotitnow\" value=\"Register\" />"+
		"</form>"
		);
    $("#wechallform").draggable({zIndex: 2500});
    $("#wechallform").click(function() {
        $("#realwechallform").submit();
    });
    $("#wechallform").show();
}

$(document).ready(function() {
    wechallinfo["form"] = "https://www.wechall.net/10-levels-on-Natas.html";
    wechallinfo["wfid"] = wechalldata[wechallinfo["level"]];

    displayWechallBox(wechallinfo);
});
