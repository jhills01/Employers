--NOTE: The first SELECT prints out result,...
--...while the 2nd one (i.e., ...INTO...) surpresses this output
--SELECT @avgStaffCntPerBranch := AVG(staffCntPerBranch)
SELECT AVG(staffCntPerBranch) INTO @avgStaffCntPerBranch
    FROM 
    (
        SELECT COUNT(*) AS staffCntPerBranch
        FROM Staff
        GROUP BY branchNo
    ) as requiredSubqueryAlias; --NOTE. this last alias, "as required...", is needed or else error

SELECT branchNo, COUNT(*) - @avgStaffCntPerBranch AS staffDiff
FROM Staff
GROUP BY branchNo
HAVING COUNT(*) > @avgStaffCntPerBranch;
--NOTE: I accidentlly used WHERE instead of HAVING. Got error....


--TEST CODE
-- SELECT @avgStaffCntPerBranch;

-- SELECT COUNT(*) AS staffCntPerBranch
-- FROM Staff
-- GROUP BY branchNo;

-- NOTE: This is wrong syntax. See above for correct way. The subquery must be in FROM clause
-- SELECT @avgStaffCntPerBranch := AVG
--     (
--         SELECT COUNT(*)
--         FROM Staff
--         GROUP BY branchNo
--     ); 

-- OLD CODE: Storing this for reference --------------------------------------------
-- DROP VIEW IF EXISTS groupByBranchNo;

-- CREATE VIEW groupByBranchNo
--     AS SELECT COUNT(*) as avgMe
--     FROM Staff
--     GROUP BY branchNo;

-- SELECT @avgStaffCntPerBranch := AVG(avgMe) FROM groupByBranchNo;

-- SELECT *
-- FROM groupByBranchNo;

-- SELECT @avgStaffCntPerBranch;