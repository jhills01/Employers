function assChks()
{
    let preventDefault = function ()
    {
        $("#assRow4").submit( //assRow4 == form
            function(key, value)
            {
                event.preventDefault();
            }
        );
    };

    let sum = 0;
    let blank = false;
    let assError = $("#assError");
    let animationSpeed = 500; // milliseconds
    let currHeight = assError.height();
    let maxHeight = "10px";
    let minHeight = "0px";
    let maxHeightInt = 10;
    let minHeightInt = 0;


    // check whether sum is 100 or not. If not, output error msg
    if (sum !== 100 && currHeight == minHeightInt)
    {
        //prevent submit default behavior
        // preventDefault();

        // output/animate error msg
        assError.text("assessment weights must add to 100");
        assError.animate({height: maxHeight}, animationSpeed);
    }
    if (blank && currHeight !== minHeightInt)
    {
        //prevent submit default behavior
        // preventDefault();

        // output/animate error msg
        assError.text("assessment weights must add to 100");
        assError.animate({height: maxHeight}, animationSpeed);
    }


}



function assWeightSumChk()
{
    // let preventDefault = function ()
    // {
    //     $("#assRow4").submit( //assRow4 == form
    //         function(key, value)
    //         {
    //             event.preventDefault();
    //         }
    //     );
    // };

    let sum = 0;
    let assError = $("#assError");
    let animationSpeed = 500; // milliseconds
    let currHeight = assError.height();
    let maxHeight = "10px";
    let minHeight = "0px";
    let maxHeightInt = 10;
    let minHeightInt = 0;

    $(".assessmentRow-1").each(
        function(index, htmlObj)
        {
            sum += parseInt(htmlObj["value"]);
        }
    );

    //NOTE: From video, we get rid of error messages when all errors are cleared.
    //      So, we leave 

    // check whether sum is 100 or not. If not, output error msg
    if (sum !== 100 && currHeight === minHeightInt)
    {
        //prevent submit default behavior
        // preventDefault();

        // output/animate error msg
        assError.text("assessment weights must add to 100");
        assError.animate({height: maxHeight}, animationSpeed);
    }
    else if (sum !== 100 && currHeight > minHeightInt)
    {
        // preventDefault();

        // just change text as error message is already showing 
        assError.text("assessment weights must add to 100");
    }
    // else
    // {
    //     // hide error message. When animation is done, function puts 
    //     assError.animate({height: minHeight}, animationSpeed, function() {assError.text("")} );
    // }

}

function assDescriptionChk()
{
    // let preventDefault = function ()
    // {
    //     $("#assRow4").submit( //assRow4 == form
    //         function(key, value)
    //         {
    //             event.preventDefault();
    //         }
    //     );
    // };

    let blank = false;
    let assError = $("#assError");
    let animationSpeed = 500; // milliseconds
    let currHeight = assError.height();
    let maxHeight = "10px";
    let minHeight = "0px";
    let maxHeightInt = 10;
    let minHeightInt = 0;

    // console.log("currHeight: ", currHeight, "type: ", typeof(currHeight));

    $(".assessmentRow-1").each(
        function(index, htmlObj)
        {
            //NOTE: not sure about this
            if (htmlObj["value"] === "") {blank = true; return false;}
        }
    );

    if (blank && currHeight !== minHeightInt)
    {
        //prevent submit default behavior
        // preventDefault();

        // output/animate error msg
        assError.text("assessment weights must add to 100");
        assError.animate({height: maxHeight}, animationSpeed);
    }
    else if(blank && currHeight > minHeightInt)
    {
        //prevent submit default behavior
        // preventDefault();

        // change error msg
        assError.text("assessment weights must add to 100");
    }
    // else
    // {
    //     // hide error message. When animation is done, function puts 
    //     assError.animate({height: minHeight}, animationSpeed, function() {assError.text("")} );

    // }
}

function assFinalChk()
{
    let preventDefault = function ()
    {
        $("#assRow4").submit( //assRow4 == form
            function(key, value)
            {
                event.preventDefault();
            }
        );
    };

    let assError = $("#assError");
    let currHeight = assError.height();
    let animationSpeed = 500; // milliseconds
    let maxHeight = "10px";
    let minHeight = "0px";
    let maxHeightInt = 10;
    let minHeightInt = 0;

    if ( currHeight > minHeightInt ) // error is showing
    {
        preventDefault();
    }
    else
    {
        assError.text("No errors! Not preventing default behavior!");
        assError.delay().animate({height: minHeight}, animationSpeed, function() {assError.text("")} );
        
    }

}

// estalish callback for checking assessment weights/ blank descriptions before submitting
$("#assRow4").submit( assWeightSumChk );
$("#assRow4").submit( assDescriptionChk );
$("#assRow4").submit( assFinalChk );