//listens for any change in value of an input element and marks it as "modified" if so
// $("input").change(
$("input").on("input",
    function() { $(this).addClass("modified"); }
);

// $("textarea").change(
$("textarea").on("input",
    function() { $(this).addClass("modified"); }
);

$(window).on('beforeunload',
    function()
    {
        let pageModified = window.sessionStorage.getItem("pageModified");
        let inputs = $("input");
        let textareas = $("textarea");
        let modificationsDetected = false;
        //NOTE: it's not possible to display custom message anymore using beforeunload
        let warning = "Unsaved input has been detected. If you leave this page, that input will be lost. Continue?";

        if (pageModified === "yes") { return warning; }

        inputs.each(
            function (index, obj)
            {
                if ( $(obj).hasClass("modified") ) 
                {modificationsDetected = true;}
            }
        );
        
        if (modificationsDetected) { return warning; }

        textareas.each(
            function (index, obj)
            {
                if ( $(obj).hasClass("modified") ) 
                {modificationsDetected = true;}
            }
        );

        if (modificationsDetected) { return warning; }
    }
);