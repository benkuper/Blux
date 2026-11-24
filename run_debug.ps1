[CmdletBinding()]
param(
    [switch]$BuildOnly
)

$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$buildDirectory = Join-Path $root 'Builds\VisualStudio2026'
$outputDirectory = Join-Path $root 'Binaries\Debug\App'
$executable = Join-Path $outputDirectory 'Blux.exe'

function Invoke-RepoGit([string]$Repository, [string[]]$GitArguments) {
    $safePath = $Repository.Replace('\', '/')
    $result = & git -c "safe.directory=$safePath" -C $Repository @GitArguments
    if ($LASTEXITCODE -ne 0) { throw "Git failed in $Repository ($($GitArguments -join ' '))." }
    return $result
}

function Get-DebugCheckout([string]$Repository, [string]$Revision, [string]$Destination, [switch]$RequireDestination) {
    $commit = Invoke-RepoGit $Repository @('rev-parse', '--verify', "$Revision^{commit}")
    if (!$RequireDestination -and (Invoke-RepoGit $Repository @('rev-parse', 'HEAD')) -eq $commit) { return $Repository }

    if (!(Test-Path -LiteralPath (Join-Path $Destination '.git'))) {
        Write-Host "Preparing $([IO.Path]::GetFileName($Destination)) at $($commit.Substring(0, 12))"
        Invoke-RepoGit $Repository @('worktree', 'add', '--quiet', '--detach', $Destination, $commit) | Out-Null
    }
    if ((Invoke-RepoGit $Destination @('rev-parse', 'HEAD')) -ne $commit) {
        throw "The debug checkout at $Destination is on the wrong revision. Expected $commit."
    }
    return $Destination
}

$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (!(Test-Path -LiteralPath $vswhere)) { throw 'Visual Studio Installer (vswhere.exe) was not found.' }
$visualStudio = & $vswhere -latest -version '[18.0,19.0)' -products '*' `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$visualStudio) { throw 'Install Visual Studio 2026 with the Desktop development with C++ workload.' }

$project = Join-Path $buildDirectory 'Blux_App.vcxproj'
if (!(Test-Path -LiteralPath $project)) { throw 'Generate the Visual Studio 2026 exporter from Blux.jucer first.' }

$cache = Join-Path $buildDirectory '.run_debug'
$null = New-Item -ItemType Directory -Path $cache -Force
$juceRepository = Join-Path (Split-Path -Parent $root) 'JUCE'
$juce = Get-DebugCheckout $juceRepository 'origin/juce8_local' (Join-Path $cache 'JUCE')
$juceModules = Join-Path $juce 'modules'

$content = Get-Content -Raw -LiteralPath $project
$content = $content.Replace('..\..\..\JUCE\modules', $juceModules)
$content = [regex]::Replace($content, '(?i)[A-Z]:\\[^;<>"\r\n]+?\\JUCE\\modules', { param($match) $juceModules })

# Build with Blux's pinned support libraries and the live OrganicUI and Timeline edits.
$moduleIncludes = @()
foreach ($module in @('juce_dmx', 'juce_serial', 'juce_simpleweb')) {
    $relative = "Modules/$module"
    $entry = Invoke-RepoGit $root @('ls-files', '--stage', '--', $relative)
    if ($entry -notmatch '^160000 ([0-9a-f]+) 0\s') { throw "No pinned revision for $relative." }
    $pin = $Matches[1]
    $repository = Join-Path $root $relative
    $destination = Join-Path $cache "Modules\$module"
    $checkout = Get-DebugCheckout $repository $pin $destination
    if ($checkout -ne $repository) {
        $content = $content.Replace("..\..\Modules\$module", $checkout)
        $moduleIncludes += Split-Path -Parent $checkout
        if ($module -eq 'juce_simpleweb') {
            $entry = Invoke-RepoGit $checkout @('ls-files', '--stage', '--', 'asio')
            if ($entry -notmatch '^160000 ([0-9a-f]+) 0\s') { throw 'No pinned revision for juce_simpleweb/asio.' }
            Get-DebugCheckout (Join-Path $repository 'asio') $Matches[1] (Join-Path $checkout 'asio') -RequireDestination | Out-Null
        }
    }
}
if ($moduleIncludes) {
    $includePath = ($moduleIncludes | Select-Object -Unique) -join ';'
    $content = $content.Replace('..\..\Modules;', "$includePath;..\..\Modules;")
}

$debugProject = Join-Path $buildDirectory 'Blux_RunDebug.vcxproj'
if (!(Test-Path -LiteralPath $debugProject) -or (Get-Content -Raw -LiteralPath $debugProject) -cne $content) {
    [IO.File]::WriteAllText($debugProject, $content)
}

$msbuild = Join-Path $visualStudio 'MSBuild\Current\Bin\amd64\MSBuild.exe'
Write-Host 'Building Blux (Debug x64)...'
& $msbuild $debugProject /nologo /verbosity:minimal /t:Build /p:Configuration=Debug /p:Platform=x64 '/p:IntDir=.run_debug\obj\' /p:CL_MPCount=4
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if (!(Test-Path -LiteralPath $executable)) { throw "Debug executable not found at $executable." }
if (!$BuildOnly) {
    Start-Process -FilePath $executable -WorkingDirectory $outputDirectory
}
