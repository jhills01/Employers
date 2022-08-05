function updateResultsTotal()
{ 
    // console.log("entering updateResultsTotal()");

    let total = 0;

    $("#resultsRow5 input[type=number]").each(
    // resultInputs.each(
        function(index, htmlObj)
        {
            total += parseInt(htmlObj["value"]);

            // console.log("index: ",index, "htmlObj:",htmlObj, "typeof(htmlObj):", typeof(htmlObj));
            // console.log("htmlObj[\"value\"]: ", htmlObj["value"] );
        }
    );

    
    // $("#resultsRow5 input[type=text]").val(total);
    $("#resultsRow5-4").val(total);
}

//NOTE: DON'T PUT FUCKING PARENTHESIS on the function you want to be called back. Just he name!!!!!!!!!!!!!!!!!!!
$("#resultsRow5 input[type=number]").on("input", updateResultsTotal );