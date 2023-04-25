# Assumptions: 

I'm assuming one version has to be selected with **maximum releaseDate** *(newest releaseDate)* for each product to calculate supportLengths.<br/>
As in example there is: <br/>
`debian 11 1053 for data.json.` <br/>
There is a cycle (version) with longer supportLength - "6", but still "11" is being selected, so I'm assuming we have to select a cycle with **the biggest releaseDate - the newest.**

### I coded a version of internship.cpp that selects a cycle (version) for each product with the longest supportLength from all of its versions, I put it in a folder called Max_Support_Length_Version.

## Important - If version doesn't have an EOL ##
I'm assuming it is still supported to this day, so logically supportLength then should be: currentSystemDate - releaseDate + 1, in case of EOL being false, EOL equal to "", EOL equal to "0" or "00000" or "0-0-0". <br/> 
Basically year && month && day have to be of value 0. <br/>
Also if there is no "eol" in JSON at all, as a row, currentSystemDate will be still taken and version will be still taken into consideration (Because there is no End of live). <br/>

If there is no year, month, day in EOL or releaseDate as they are equal to 0, but one of these is not 0 (or two), then the version will be skipped, because I'm assuming this kind of date is unvalid - <ins>how can a date 2024-0-0 be logically expressed?</ins> <br/>
If date is 0-0-0 then a logical conclusion is that it doesn't exist. <br/>
### There is a difference between how unvalid dates and dates that don't exist are handled. <br/> ###
Invalid date will cause version to not be taken into consideration (either unvalid EOL or ReleaseDate). <br/>
On the other hand if date doesn't exist there are 2 scenarios: <br/>
- It's EOL and currentSystemTime will be taken instead (because EOL doesn't exist, product is still supported to this day) <br/>
- It's ReleaseDate and since version has no date the logical conclusion is that it was never released (so version won't be taken into consideration) <br/>

### Example of unvalid dates:

* 2022
* 05-10
* 10
* 0-0-10
* 0-12-10
* 2022-10-0
* 2022-10-
* !$%-12-10 (if dates are not numeric then version will be skipped)
* +2022-10-02 ('+' makes date not numeric)
* 20 22-10-02 (' ' makes date not numeric)

0-0-0, "000", "" for releasedDate is considered as version was never released, so it's skipped.
For EOL on the other hand currentSystemDate will be taken instead. 

If releaseDates are equal then the version that is higher, earlier in JSON will be picked.

Versions that only belong to product with OS value of boolean true are only considered.

Unit tests are for maximum releaseDate version (that internship.cpp in src).

# Edge cases:

- switching to "releasedDate" for data1.json.
- Dates can be random: ex. "!!!!!" - (added check if it's numeric, doesn't have ('-') (negative) and try catch for overflowing).
- releaseDate can be empty, equal to zero, "0000"... So i guess version (cycle) doesn't exist - it was never released. - (skipping version (cycle) in that case).
- EOL can be empty. - (taking currentDate instead, because version is still supported (there is no EOL)).
- Dates can have only have year. - unvalid date, will be skipped, not taken into consideration.
- Dates can have only year or month or day. or combinations of these but missing something - unvalid date, version will be not taken into consideration. (ex 2024, 2012-0-12, 12-05, etc.)
- "+2022-+10-+2" considered as unvalid in checkIfConvertable.
- checking if month is in <1; 12>, if not then skipping (invalid month), checking if day is in <1; 31>, valid for a month (skipping if invalid day or month).
- Checking leap years for February also.
- I'm not sure what to do if there are whitespaces, ex. "12 345", when converted to uint it would be 12, so if I would want to consider this I would remove them before converting, I'm going to simply assume it has to be skipped as ' ' is not numeric. So date like "20 27-05-21" will be unvalid.
- If there is no "eol" then currentDate will be taken instead, as product is still supported.
- If there is no  "releaseDate" (or "releasedDate"), ex. there is "releaseDats", then the version will be skipped, since it was never released. 
- if there is no "os", ex. "oss" : true, then product will be skipped.
- if there is no "name", ex. "names", then if checking, continuing, so program doesn't crash, product will be skipped.
- if every version is invalid is some way (continue triggered), or versions list is empty, then not adding supportLength = 0 to results, I made a single boolean for that.

### Other Assumptions:

- I don't know what logic to implement if EOL is true, but none of the OS have EOL true. I just skip version if EOL is true.
