#parameter: path to file
#return: m2tweop version string
#using: findEOPVersion.ps1 
param (
    [string]$filePath = ""
)

$eopVerLine=""
$regex=".*eopVersionName.*"".*"""
foreach($line in Get-Content $filePath) {
    if($line -match $regex){
		$eopVerPos=$line.IndexOf("""")
		if ( $eopVerPos -eq -1 )
		{
			continue
		}
		$eopVerPos=$eopVerPos+1
		$eopVerPos2=$line.LastIndexOf("""")
		if ($eopVerPos2 -eq -1 )
		{
			continue
		}
		$eopVerLine=$line.Substring($eopVerPos,$eopVerPos2-$eopVerPos)

		break
    }
}
return $eopVerLine