param
(
    [Parameter( Mandatory = $true )]
    [string] $Config,

    [Parameter( Mandatory = $true )]
    [string] $Target
);


$ObjectDir = "build/" + $Config + "/tmp";
$OutputDir = "build/" + $Config + "/out";
$SourceDir = "source/" + $Target;


if( ( Test-Path -Path $ObjectDir ) -eq $false )
{
    New-Item -Path $ObjectDir -ItemType Directory | Out-Null; Write-Host "Object directory is created.";
}

if( ( Test-Path -Path $OutputDir ) -eq $false )
{
    New-Item -Path $OutputDir -ItemType Directory | Out-Null; Write-Host "Output directory is created.";
}


$Compiler = "clang++";
$Flags    = "-c -std=c++26 -ggdb -O0 -I include";
$Sources  = @( );
$Objects  = @( );

foreach( $File in ( Get-ChildItem -Path $SourceDir ) )
{
    $Name = $File.Name.Substring( 0, $File.Name.LastIndexOf( "." ) );

    $Sources += ( $SourceDir + "/" + $Name + ".cc" );
    $Objects += ( $ObjectDir + "/" + $Name + ".o"  );
}

for( $I = 0; $I -lt $Sources.Length; $I++ )
{
    $Command = $Compiler + ' -o "' + $Objects[ $I ] + '" "' + $Sources[ $I ] + '" ' + $Flags;

    Write-Host        $Command;
    Invoke-Expression $Command;
}


$Command = $Compiler + ' -o "' + $OutputDir + '/' + $Target + '"';

foreach( $Object in $Objects )
{
    $Command += ' ';
    $Command += $Object;
}

Write-Host        $Command;
Invoke-Expression $Command;