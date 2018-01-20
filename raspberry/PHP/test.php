<?
session_start();
$_SESSION['capnum'] = ((isset($_SESSION['capnum'])) ? $_SESSION['capnum'] : 0);
if(isset($_GET['add'])){
     $_SESSION['capnum']++;
}
?>
<form action="<? echo $_SERVER['PHP_SELF']; ?>" method="get">
<input type="submit" name="add" value="add" />
</form>
<?
echo $_SESSION['capnum'];
?>