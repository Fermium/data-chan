# launch a powershell subprocess, to reload things. Otherwise you'll get "choco command not found"
cmd.exe /c start powershell.exe -c { Set-Location $PWD } -NoExit
