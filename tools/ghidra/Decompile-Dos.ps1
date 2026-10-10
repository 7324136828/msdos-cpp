param(
    [Parameter(Mandatory=$true)][string]$GhidraHome,
    [Parameter(Mandatory=$true)][string]$Binary,
    [string]$OutputDirectory
)
$ErrorActionPreference = 'Stop'
$projectRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
if (-not $OutputDirectory) { $OutputDirectory = Join-Path $projectRoot 'decompiled' }
$binaryPath = (Resolve-Path -LiteralPath $Binary).Path
$extension = [IO.Path]::GetExtension($binaryPath).ToLowerInvariant()
if ($extension -notin '.com','.exe') { throw 'Only DOS COM/EXE input is accepted.' }
$headless = Join-Path $GhidraHome 'support/analyzeHeadless.bat'
if (-not (Test-Path -LiteralPath $headless)) { throw 'Ghidra analyzeHeadless.bat not found.' }
$outputRoot = [IO.Path]::GetFullPath($OutputDirectory)
New-Item -ItemType Directory -Path $outputRoot -Force | Out-Null
$projectDirectory = Join-Path $projectRoot 'tools/ghidra/projects'
New-Item -ItemType Directory -Path $projectDirectory -Force | Out-Null
$projectName = 'dos-' + [Guid]::NewGuid().ToString('N')
$outputFile = Join-Path $outputRoot (([IO.Path]::GetFileName($binaryPath)) + '.c')
if (Test-Path -LiteralPath $outputFile) { throw "Output already exists: $outputFile" }
$headlessArguments = @($projectDirectory,$projectName,'-import',$binaryPath,
    '-processor','x86:LE:16:Real Mode','-scriptPath',$PSScriptRoot,
    '-postScript','ExportDOS.java',$outputFile,$extension.Substring(1),'-deleteProject')
if ($extension -eq '.com') { $headlessArguments += @('-loader','BinaryLoader','-loader-baseAddr','0x100') }
& $headless @headlessArguments
if ($LASTEXITCODE -ne 0) { throw "Ghidra failed with exit code $LASTEXITCODE" }
if (-not (Test-Path -LiteralPath $outputFile)) { throw 'Ghidra did not produce the expected export.' }
Write-Output $outputFile
