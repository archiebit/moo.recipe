param
(
    [Parameter( Mandatory = $true )]
    [string] $Config
);


$ConfigDir = "build/" + $Config;

if( Test-Path -Path $ConfigDir )
{
    Remove-Item -Path $ConfigDir -Recurse -Force;
}