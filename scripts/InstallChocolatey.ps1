 $ChocoInstallPath = "$env:SystemDrive\ProgramData\Chocolatey\bin"

# check if chocolatey exists					
  if (!(Test-Path $ChocoInstallPath)) {
      iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))
  }
