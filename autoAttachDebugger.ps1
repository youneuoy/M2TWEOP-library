# Define the process name and Visual Studio paths
$processName = "medieval2"
$vsPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"
$vsSolution = 'C:\Users\stead\Documents\Coding Projects\M2TWEOP-library\M2TWEOP Code\M2TWEOP library.sln'

# Log message to confirm that script is starting
Write-Host "Monitoring for process: $processName..."

while ($true) {
    $process = Get-Process | Where-Object { $_.Name -eq $processName } -ErrorAction SilentlyContinue
    
    if ($process) {
        Write-Host "Process $processName found. Process ID: $($process.Id)"
        
        # Step 1: Open Visual Studio and load the solution
        Write-Host "Opening solution in Visual Studio..."
        $vsInstance = Start-Process $vsPath -ArgumentList "`"$vsSolution`"" -PassThru

        # Wait to ensure Visual Studio opens the solution
        Start-Sleep -Seconds 10
        
        # Step 2: Use Visual Studio's DTE to attach the debugger to the process
        Write-Host "Attaching debugger to process ID: $($process.Id)..."
        $dte = [Runtime.Interopservices.Marshal]::GetActiveObject("VisualStudio.DTE")
        $processToAttach = $dte.Debugger.LocalProcesses | Where-Object { $_.ProcessID -eq $process.Id }
        
        if ($processToAttach) {
            $processToAttach.Attach()
            Write-Host "Debugger attached successfully to process $processName (ID: $($process.Id))"
        } else {
            Write-Host "Failed to find the process to attach the debugger."
        }
        
        # Exit the loop since the process was found and debugger was attached
        break
    }
    
    # Log message indicating that the process is still not found
    # Write-Host "Process $processName not found. Retrying in 1 second..."
    # Start-Sleep -Seconds 1
}

Write-Host "Script complete."
