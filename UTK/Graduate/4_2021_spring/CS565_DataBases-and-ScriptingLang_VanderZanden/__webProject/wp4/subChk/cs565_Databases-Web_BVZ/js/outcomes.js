//WAIT WAIT WAIT WAIT WAIT
// THIS WILL NOT WORK!!!!
// If I add more ajaxStop() in other files and put them in abet.php it will muddy the waters since I'll...
// ...be calling ajax AGAIN in those files. 
// SOLUTION: The only way to do this this right is to encapsulate all code in outcomes.js and others in a... 
//           ... function and then call that function in .done() of the preceding ajax call. However, this... 
//           ... is too convoluted for not much gain. Just use PHP and include to do all the work. It'll be...
//           ...  easier. 

// -----------------------------------------------------------------------------------------------------------

// IMPORTANT: $(document).ajaxStop() registers a callback function WHEN ALL AJAX REQUESTS ARE COMPLETE!!!!
//            This is very very very important for my strategy 

// WAIT WAIT WAIT
// I think I've got a major problem here! This function will probably be called everytime 
// ...ajax stops for any requests@!!!!! 
// HA!!! LOOK BELOW. THIS IS SOLVED!!!
$(document).ajaxStop(
    function()
    {
        // console.log("Did I print last?");
        let outcomes = window.sessionStorage.getItem("outcomes");

        console.log(outcomes);
        console.log(typeof(outcomes));

        // outcomes = JSON.stringify(outcomes);
        outcomes = JSON.parse(outcomes);

        console.log(outcomes);
        console.log(typeof(outcomes));

        outcomes.forEach(
            function(obj)
            {
                obj = JSON.parse(obj);
                console.log(obj);
            }
        );
    }
);

//NICE!! This unbinds the anonymous function above so I can re-use it!!!!!!!!!!!!! 
$(document).off("ajaxStop");
