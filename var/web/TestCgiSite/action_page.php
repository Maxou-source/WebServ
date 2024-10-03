<!DOCTYPE html>
<html>
<head>
    <title>QUERY_STRING Output</title>
</head>
<body>

<h1>QUERY_STRING Output</h1>

<div>
    <?php
    // Check if QUERY_STRING is set
    // if (isset($_SERVER['QUERY_STRING'])) {
    //     // Get and display the value of QUERY_STRING
    //     $queryString = $_SERVER['QUERY_STRING'];
    //     echo "<p>QUERY_STRING: $queryString</p>";
    // } else {
    //     echo "<p>QUERY_STRING is not set.</p>";
    // }
    foreach ($_SERVER as $key => $value) {
        echo $key . " => " . $value . "<br>";
    }
    ?>
</div>
</body>
</html>