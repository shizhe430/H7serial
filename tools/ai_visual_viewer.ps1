param(
    [string]$Port = "",
    [int]$Baud = 921600,
    [string]$SaveDir = "",
    [int]$RoiOffsetX = 50,
    [int]$RoiOffsetY = -1,
    [int]$ViewShiftX = 0,
    [int]$ViewShiftY = 0,
    [int]$ViewFillValue = 180
)

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

$script:HeaderSize = 32
$script:Magic = [byte[]](0x41, 0x49, 0x56, 0x31)
$script:TailMagic = [byte[]](0x41, 0x49, 0x45, 0x31)
$script:Buffer = New-Object 'System.Collections.Generic.List[byte]'
$script:SerialPort = $null
$script:LastFrameTick = $null
$script:LastFrameId = $null
$script:DisplayFps = 0.0
$script:TotalBytesReceived = 0L
$script:LastTextLog = ""
$script:LastDiag = ""
$script:LastFailedJpegPath = Join-Path $env:TEMP "h7_ai_visual_last_fail.jpg"
$script:CurrentFrame = $null
$script:AutoSaveEnabled = $false
$script:SavedCount = 0
$script:SelectedSaveClass = "label_0_no_cup"
$script:SaveDir = if ([string]::IsNullOrWhiteSpace($SaveDir)) {
    Join-Path ([Environment]::GetFolderPath("MyPictures")) "h7_ai_dataset"
} else {
    $SaveDir
}

function Get-LeUInt16 {
    param([byte[]]$Buffer, [int]$Offset)
    return [uint16]([int]$Buffer[$Offset] -bor ([int]$Buffer[$Offset + 1] -shl 8))
}

function Get-LeUInt32 {
    param([byte[]]$Buffer, [int]$Offset)
    return [uint32](
        ([int]$Buffer[$Offset]) -bor
        ([int]$Buffer[$Offset + 1] -shl 8) -bor
        ([int]$Buffer[$Offset + 2] -shl 16) -bor
        ([int]$Buffer[$Offset + 3] -shl 24)
    )
}

function Get-SignedByte {
    param([byte]$Value)
    if ($Value -ge 128) {
        return ([int]$Value - 256)
    }
    return [int]$Value
}

function Get-VisualLogit {
    param([byte]$Value)
    return (([double][int]$Value - 128.0) / 4.0)
}

function Format-HexBytes {
    param([byte[]]$Bytes)
    return (($Bytes | ForEach-Object { $_.ToString("X2") }) -join ' ')
}

function Append-TextDiagnostic {
    param([byte[]]$Bytes)

    if (($null -eq $Bytes) -or ($Bytes.Length -eq 0)) {
        return
    }

    $text = [System.Text.Encoding]::ASCII.GetString($Bytes)
    $text = [System.Text.RegularExpressions.Regex]::Replace(
        $text,
        '[^\x09\x0A\x0D\x20-\x7E]',
        '.'
    )
    $script:LastTextLog += $text
    if ($script:LastTextLog.Length -gt 4000) {
        $script:LastTextLog = $script:LastTextLog.Substring($script:LastTextLog.Length - 4000)
    }
}

function To-SafeText {
    param($Value)
    if ($null -eq $Value) {
        return ""
    }
    if ($Value -is [System.Array]) {
        return [string]::Join(" | ", ($Value | ForEach-Object { [string]$_ }))
    }
    return [string]$Value
}

function Format-PathForDisplay {
    param(
        [string]$PathText,
        [int]$MaxLen = 44
    )

    if ([string]::IsNullOrWhiteSpace($PathText)) {
        return ""
    }
    if ($PathText.Length -le $MaxLen) {
        return $PathText
    }
    $keep = [Math]::Max(8, [int](($MaxLen - 3) / 2))
    return "{0}...{1}" -f $PathText.Substring(0, $keep), $PathText.Substring($PathText.Length - $keep)
}

function Get-ClassName {
    param([int]$ClassId)
    switch ($ClassId) {
        0 { return "no_cup" }
        1 { return "low" }
        2 { return "half" }
        3 { return "full" }
        4 { return "abnormal" }
        default { return "unknown" }
    }
}

function Get-ClassLabel {
    param([int]$ClassId)
    switch ($ClassId) {
        0 { return "NO CUP" }
        1 { return "LOW" }
        2 { return "HALF" }
        3 { return "FULL" }
        4 { return "ABNORMAL" }
        default { return "UNKNOWN" }
    }
}

function Get-ClassColor {
    param([int]$ClassId)
    switch ($ClassId) {
        0 { return [System.Drawing.Color]::Goldenrod }
        1 { return [System.Drawing.Color]::DeepSkyBlue }
        2 { return [System.Drawing.Color]::MediumSeaGreen }
        3 { return [System.Drawing.Color]::OrangeRed }
        4 { return [System.Drawing.Color]::Crimson }
        default { return [System.Drawing.Color]::Gray }
    }
}

function Ensure-SaveDir {
    param([string]$TargetPath = $script:SaveDir)
    if (-not (Test-Path -LiteralPath $TargetPath)) {
        New-Item -ItemType Directory -Path $TargetPath -Force | Out-Null
    }
}

function Get-FrameFileStem {
    param($Frame)

    $timeText = [DateTime]::Now.ToString("yyyyMMdd_HHmmss_fff")
    $confMilli = [Math]::Max(0, [Math]::Min(9999, [int]([Math]::Round($Frame.Confidence * 1000.0))))
    $regMilli = [Math]::Max(0, [Math]::Min(9999, [int]([Math]::Round($Frame.Regression * 1000.0))))
    $stem = "{0}_fid{1}_cls{2}_raw{3}_c{4:D4}_r{5:D4}" -f $timeText, $Frame.FrameId, $Frame.ClassId, $Frame.RawClassId, $confMilli, $regMilli
    if ($Frame.HasLightMetadata) {
        $stem += "_light{0:D4}" -f $Frame.LightDuty
    }
    return $stem
}

function Get-ClassFolderName {
    param($Frame)
    return "label_{0}_{1}" -f $Frame.ClassId, $Frame.ClassName
}

function Get-SaveClassOptions {
    return @(
        "label_0_no_cup",
        "label_1_low",
        "label_2_half",
        "label_3_full",
        "label_4_abnormal"
    )
}

function Get-SelectedSaveFolderName {
    if ($null -ne $comboSaveClass -and $comboSaveClass.SelectedItem) {
        return [string]$comboSaveClass.SelectedItem
    }
    return $script:SelectedSaveClass
}

function Get-SelectedSaveFolderPath {
    $folderName = Get-SelectedSaveFolderName
    $targetDir = Join-Path $script:SaveDir $folderName
    Ensure-SaveDir -TargetPath $targetDir
    return $targetDir
}

function Get-SavedPhotoCount {
    $targetDir = Get-SelectedSaveFolderPath
    return @(Get-ChildItem -LiteralPath $targetDir -Filter *.jpg -File -ErrorAction SilentlyContinue).Count
}

function Update-SaveUi {
    $script:SelectedSaveClass = Get-SelectedSaveFolderName
    if ($null -ne $textSaveRoot) {
        $textSaveRoot.Text = $script:SaveDir
    }
    if ($null -ne $labelTargetFolder) {
        $labelTargetFolder.Text = "Folder: $script:SelectedSaveClass"
    }
    $script:SavedCount = Get-SavedPhotoCount
    if ($null -ne $labelSaveCount) {
        $labelSaveCount.Text = "Photos: $($script:SavedCount)"
    }
}

function Save-FrameArtifacts {
    param($Frame)

    if ($null -eq $Frame) {
        Set-Status "no frame to save"
        return
    }

    try {
        Ensure-SaveDir
        $targetDir = Get-SelectedSaveFolderPath
        $stem = Get-FrameFileStem -Frame $Frame
        $jpgPath = Join-Path $targetDir ($stem + ".jpg")
        [System.IO.File]::WriteAllBytes($jpgPath, $Frame.Jpeg)
        Update-SaveUi
        Set-Status "saved $jpgPath"
    } catch {
        Set-Status "save failed: $($_.Exception.Message)"
    }
}

function Find-MagicIndex {
    param([System.Collections.Generic.List[byte]]$Buffer)
    for ($i = 0; $i -le ($Buffer.Count - $script:Magic.Length); $i++) {
        $match = $true
        for ($j = 0; $j -lt $script:Magic.Length; $j++) {
            if ($Buffer[$i + $j] -ne $script:Magic[$j]) {
                $match = $false
                break
            }
        }
        if ($match) {
            return $i
        }
    }
    return -1
}

function Try-ExtractFrame {
    if ($script:Buffer.Count -lt $script:HeaderSize) {
        return $null
    }

    $magicIndex = Find-MagicIndex -Buffer $script:Buffer
    if ($magicIndex -lt 0) {
        if ($script:Buffer.Count -gt 3) {
            $discardCount = $script:Buffer.Count - 3
            Append-TextDiagnostic -Bytes $script:Buffer.GetRange(0, $discardCount).ToArray()
            $script:Buffer.RemoveRange(0, $discardCount)
        }
        return $null
    }

    if ($magicIndex -gt 0) {
        Append-TextDiagnostic -Bytes $script:Buffer.GetRange(0, $magicIndex).ToArray()
        $script:Buffer.RemoveRange(0, $magicIndex)
    }

    if ($script:Buffer.Count -lt $script:HeaderSize) {
        return $null
    }

    $header = $script:Buffer.GetRange(0, $script:HeaderSize).ToArray()
    if ($header[4] -ne 0x01) {
        $script:Buffer.RemoveAt(0)
        return $null
    }
    $jpegLen = [int](Get-LeUInt32 -Buffer $header -Offset 16)
    if (($jpegLen -le 0) -or ($jpegLen -gt 524288)) {
        $script:Buffer.RemoveAt(0)
        return $null
    }

    if ($script:Buffer.Count -lt ($script:HeaderSize + $jpegLen + $script:TailMagic.Length)) {
        return $null
    }

    $jpeg = $script:Buffer.GetRange($script:HeaderSize, $jpegLen).ToArray()
    $tail = $script:Buffer.GetRange($script:HeaderSize + $jpegLen, $script:TailMagic.Length).ToArray()
    if (($jpeg.Length -lt 4) -or
        ($jpeg[0] -ne 0xFF) -or
        ($jpeg[1] -ne 0xD8) -or
        ($jpeg[$jpeg.Length - 2] -ne 0xFF) -or
        ($jpeg[$jpeg.Length - 1] -ne 0xD9)) {
        $script:Buffer.RemoveAt(0)
        return $null
    }

    for ($i = 0; $i -lt $script:TailMagic.Length; $i++) {
        if ($tail[$i] -ne $script:TailMagic[$i]) {
            $script:Buffer.RemoveAt(0)
            return $null
        }
    }

    $script:Buffer.RemoveRange(0, $script:HeaderSize + $jpegLen + $script:TailMagic.Length)

    $protocolFlags = [int]$header[7]
    $hasRoiMetadata = (($protocolFlags -band 0x04) -ne 0)
    $hasLightMetadata = (($protocolFlags -band 0x08) -ne 0)

    return [pscustomobject]@{
        Version      = [int]$header[4]
        ClassId      = [int]$header[5]
        RawClassId   = [int]$header[6]
        InputHash    = $protocolFlags
        Confidence   = (Get-LeUInt16 -Buffer $header -Offset 8) / 1000.0
        Regression   = (Get-LeUInt16 -Buffer $header -Offset 10) / 1000.0
        PipelineMs   = [int](Get-LeUInt16 -Buffer $header -Offset 12)
        InferMs      = [int](Get-LeUInt16 -Buffer $header -Offset 14)
        JpegLen      = $jpegLen
        FrameId      = [int](Get-LeUInt32 -Buffer $header -Offset 20)
        Logits       = @(
            (Get-VisualLogit -Value $header[24]),
            (Get-VisualLogit -Value $header[25]),
            (Get-VisualLogit -Value $header[26]),
            (Get-VisualLogit -Value $header[27]),
            (Get-VisualLogit -Value $header[28])
        )
        InputMin     = ([int]$header[29] - 128)
        InputMax     = ([int]$header[30] - 128)
        InputMean    = ([int]$header[31] - 128)
        HasInputStats = ((-not $hasRoiMetadata) -and (-not $hasLightMetadata) -and (($header[29] -ne 0) -or ($header[30] -ne 0) -or ($header[31] -ne 0)))
        HasRoiMetadata = $hasRoiMetadata
        HasLightMetadata = $hasLightMetadata
        RoiOffsetX   = if ($hasRoiMetadata) { ([int]$header[29] - 128) } else { $RoiOffsetX }
        RoiOffsetY   = if ($hasRoiMetadata) { ([int]$header[30] - 128) } else { $RoiOffsetY }
        ViewFill     = if ($hasRoiMetadata -and (-not $hasLightMetadata)) { [int]$header[31] } else { $ViewFillValue }
        LightDuty    = if ($hasLightMetadata) { [int]$header[31] * 4 } else { -1 }
        ClassName    = Get-ClassName -ClassId ([int]$header[5])
        JpegHead     = if ($jpegLen -ge 4) { Format-HexBytes -Bytes $jpeg[0..3] } else { "" }
        JpegTail     = if ($jpegLen -ge 2) { Format-HexBytes -Bytes $jpeg[($jpegLen - 2)..($jpegLen - 1)] } else { "" }
        TailMagic    = Format-HexBytes -Bytes $tail
        Jpeg         = $jpeg
    }
}

function Refresh-PortList {
    $portNames = [System.IO.Ports.SerialPort]::GetPortNames() | Sort-Object
    $comboPorts.Items.Clear()
    foreach ($name in $portNames) {
        [void]$comboPorts.Items.Add($name)
    }
    if ($Port -and $comboPorts.Items.Contains($Port)) {
        $comboPorts.SelectedItem = $Port
    } elseif ($comboPorts.Items.Count -gt 0 -and -not $comboPorts.SelectedItem) {
        $comboPorts.SelectedIndex = 0
    }
}

function Set-Status {
    param([string]$Text)
    $labelStatus.Text = "Status: $Text"
}

function Disconnect-Serial {
    if ($script:SerialPort -ne $null) {
        try {
            if ($script:SerialPort.IsOpen) {
                $script:SerialPort.Close()
            }
            $script:SerialPort.Dispose()
        } catch {
        }
        $script:SerialPort = $null
    }
    Set-Status "disconnected"
}

function Connect-Serial {
    Disconnect-Serial

    if (-not $comboPorts.SelectedItem) {
        Set-Status "no COM port selected"
        return
    }

    try {
        $script:SerialPort = New-Object System.IO.Ports.SerialPort($comboPorts.SelectedItem, $Baud, [System.IO.Ports.Parity]::None, 8, [System.IO.Ports.StopBits]::One)
        $script:SerialPort.ReadTimeout = 20
        $script:SerialPort.ReadBufferSize = 1048576
        $script:SerialPort.Open()
        $script:Buffer.Clear()
        $script:TotalBytesReceived = 0L
        $script:LastTextLog = ""
        $script:LastFrameTick = $null
        $script:LastFrameId = $null
        $script:DisplayFps = 0.0
        Set-Status "connected to $($comboPorts.SelectedItem) @ $Baud"
    } catch {
        Disconnect-Serial
        Set-Status "connect failed: $($_.Exception.Message)"
    }
}

function Update-Viewer {
    param($Frame)

    try {
        $script:CurrentFrame = $Frame
        $memory = New-Object System.IO.MemoryStream
        $memory.Write($Frame.Jpeg, 0, $Frame.Jpeg.Length)
        $memory.Position = 0
        $sourceBitmap = [System.Drawing.Bitmap]::new([System.IO.Stream]$memory)
        $sourceWidth = [int]$sourceBitmap.Width
        $sourceHeight = [int]$sourceBitmap.Height
        $bitmap = [System.Drawing.Bitmap]::new($sourceWidth, $sourceHeight)
        $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
        $graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias
        $fill = [System.Drawing.Color]::FromArgb($Frame.ViewFill, $Frame.ViewFill, $Frame.ViewFill)
        $graphics.Clear($fill)
        $graphics.DrawImage($sourceBitmap, 0, 0, $sourceWidth, $sourceHeight)

        $centerX = [int]($sourceWidth / 2)
        if (($sourceWidth -eq 320) -and ($sourceHeight -eq 240)) {
            $centerX += $Frame.RoiOffsetX
        }
        $centerY = [int]($sourceHeight / 2)
        if (($sourceWidth -eq 320) -and ($sourceHeight -eq 240)) {
            $centerY += $Frame.RoiOffsetY
        }
        $radius = if (($sourceWidth -eq 320) -and ($sourceHeight -eq 240)) {
            100
        } elseif (($sourceWidth -eq 224) -and ($sourceHeight -eq 224)) {
            100
        } else {
            [int]([Math]::Min($sourceWidth, $sourceHeight) * 0.42)
        }
        $penColor = Get-ClassColor -ClassId $Frame.ClassId
        $pen = [System.Drawing.Pen]::new($penColor, 3.0)
        $roiLeft = [int]($centerX - $radius)
        $roiTop = [int]($centerY - $radius)
        $roiSize = [int]($radius * 2)
        $roiRect = [System.Drawing.Rectangle]::new($roiLeft, $roiTop, $roiSize, $roiSize)
        $graphics.DrawEllipse($pen, $roiRect)

        $previousImage = $pictureBox.Image
        $pictureBox.Image = $bitmap
        if ($previousImage -ne $null) {
            $previousImage.Dispose()
        }

        $labelClassValue.Text = Get-ClassLabel -ClassId $Frame.ClassId
        $labelClassValue.BackColor = Get-ClassColor -ClassId $Frame.ClassId
        $labelClass.Text = "Class: $($Frame.ClassName) ($($Frame.ClassId))"
        $labelRawClass.Text = "Raw class: $($Frame.RawClassId)"
        $labelConfidence.Text = ("Confidence: {0:P1}" -f $Frame.Confidence)
        $labelRegression.Text = ("Regression: {0:F3}" -f $Frame.Regression)
        $labelPipeline.Text = "Pipeline: $($Frame.PipelineMs) ms"
        $labelInfer.Text = "Infer: $($Frame.InferMs) ms"
        $labelFps.Text = ("FPS: {0:F1}" -f $script:DisplayFps)
        $labelFrame.Text = "Frame ID: $($Frame.FrameId)"
        $lightText = if ($Frame.HasLightMetadata) { "  Light=$($Frame.LightDuty)/999" } else { "" }
        $labelJpeg.Text = "JPEG: ${sourceWidth}x${sourceHeight}, $($Frame.JpegLen) bytes  ROI=($($Frame.RoiOffsetX),$($Frame.RoiOffsetY))$lightText"
        if ($Frame.HasInputStats) {
            $labelLogits.Text = "Logits: $($Frame.Logits -join ', ')`r`nInput q: min=$($Frame.InputMin) max=$($Frame.InputMax) mean=$($Frame.InputMean) hash=$($Frame.InputHash)"
        } else {
            $labelLogits.Text = "Logits: $($Frame.Logits -join ', ')"
        }
        $progressConfidence.Value = [Math]::Max(0, [Math]::Min(1000, [int]($Frame.Confidence * 1000.0)))
        $progressWater.Value = [Math]::Max(0, [Math]::Min(1000, [int]($Frame.Regression * 1000.0)))

        $graphics.Dispose()
        $pen.Dispose()
        $sourceBitmap.Dispose()
        $memory.Dispose()
    } catch {
        $script:LastDiag = "decode fail len=$($Frame.JpegLen) head=$($Frame.JpegHead) tail=$($Frame.JpegTail) end=$($Frame.TailMagic)"
        try {
            [System.IO.File]::WriteAllBytes($script:LastFailedJpegPath, $Frame.Jpeg)
        } catch {
        }
        $errType = To-SafeText $_.Exception.GetType().FullName
        $errMsg = To-SafeText $_.Exception.Message
        Set-Status ("{0} ex={1}: {2}" -f $script:LastDiag, $errType, $errMsg)
    }
}

$form = New-Object System.Windows.Forms.Form
$form.Text = "H7 AI Visual Viewer"
$form.Width = 1360
$form.Height = 860
$form.StartPosition = "CenterScreen"
$form.BackColor = [System.Drawing.Color]::FromArgb(245, 247, 250)

$panelTop = New-Object System.Windows.Forms.Panel
$panelTop.Dock = "Top"
$panelTop.Height = 52
$panelTop.BackColor = [System.Drawing.Color]::FromArgb(20, 26, 36)
$form.Controls.Add($panelTop)

$buttonFont = New-Object System.Drawing.Font("Segoe UI", 9, [System.Drawing.FontStyle]::Regular)
$valueFont = New-Object System.Drawing.Font("Segoe UI", 10, [System.Drawing.FontStyle]::Regular)
$monoFont = New-Object System.Drawing.Font("Consolas", 9, [System.Drawing.FontStyle]::Regular)

$comboPorts = New-Object System.Windows.Forms.ComboBox
$comboPorts.Left = 12
$comboPorts.Top = 11
$comboPorts.Width = 140
$comboPorts.Font = $buttonFont
$panelTop.Controls.Add($comboPorts)

$buttonRefresh = New-Object System.Windows.Forms.Button
$buttonRefresh.Text = "Refresh"
$buttonRefresh.Left = 164
$buttonRefresh.Top = 9
$buttonRefresh.Width = 96
$buttonRefresh.Height = 28
$buttonRefresh.Font = $buttonFont
$buttonRefresh.FlatStyle = [System.Windows.Forms.FlatStyle]::Standard
$buttonRefresh.Add_Click({ Refresh-PortList })
$panelTop.Controls.Add($buttonRefresh)

$buttonConnect = New-Object System.Windows.Forms.Button
$buttonConnect.Text = "Connect"
$buttonConnect.Left = 272
$buttonConnect.Top = 9
$buttonConnect.Width = 96
$buttonConnect.Height = 28
$buttonConnect.Font = $buttonFont
$buttonConnect.FlatStyle = [System.Windows.Forms.FlatStyle]::Standard
$buttonConnect.Add_Click({ Connect-Serial })
$panelTop.Controls.Add($buttonConnect)

$buttonDisconnect = New-Object System.Windows.Forms.Button
$buttonDisconnect.Text = "Disconnect"
$buttonDisconnect.Left = 380
$buttonDisconnect.Top = 9
$buttonDisconnect.Width = 104
$buttonDisconnect.Height = 28
$buttonDisconnect.Font = $buttonFont
$buttonDisconnect.FlatStyle = [System.Windows.Forms.FlatStyle]::Standard
$buttonDisconnect.Add_Click({ Disconnect-Serial })
$panelTop.Controls.Add($buttonDisconnect)

$buttonSave = New-Object System.Windows.Forms.Button
$buttonSave.Text = "Save JPEG"
$buttonSave.Left = 492
$buttonSave.Top = 9
$buttonSave.Width = 96
$buttonSave.Height = 28
$buttonSave.Font = $buttonFont
$buttonSave.FlatStyle = [System.Windows.Forms.FlatStyle]::Standard
$buttonSave.Add_Click({ Save-FrameArtifacts -Frame $script:CurrentFrame })
$panelTop.Controls.Add($buttonSave)

$checkAutoSave = New-Object System.Windows.Forms.CheckBox
$checkAutoSave.Text = "Auto Save JPEG"
$checkAutoSave.Left = 602
$checkAutoSave.Top = 14
$checkAutoSave.Width = 118
$checkAutoSave.Font = $buttonFont
$checkAutoSave.ForeColor = [System.Drawing.Color]::White
$checkAutoSave.Add_CheckedChanged({
    $script:AutoSaveEnabled = $checkAutoSave.Checked
    if ($script:AutoSaveEnabled) {
        Update-SaveUi
        Set-Status "auto save on -> $script:SelectedSaveClass"
    } else {
        Set-Status "auto save off"
    }
})
$panelTop.Controls.Add($checkAutoSave)

$labelSaveClassTop = New-Object System.Windows.Forms.Label
$labelSaveClassTop.Text = "Label"
$labelSaveClassTop.Left = 730
$labelSaveClassTop.Top = 15
$labelSaveClassTop.Width = 42
$labelSaveClassTop.Font = $buttonFont
$labelSaveClassTop.ForeColor = [System.Drawing.Color]::White
$panelTop.Controls.Add($labelSaveClassTop)

$comboSaveClass = New-Object System.Windows.Forms.ComboBox
$comboSaveClass.Left = 778
$comboSaveClass.Top = 11
$comboSaveClass.Width = 136
$comboSaveClass.Font = $buttonFont
$comboSaveClass.DropDownStyle = [System.Windows.Forms.ComboBoxStyle]::DropDownList
foreach ($item in (Get-SaveClassOptions)) {
    [void]$comboSaveClass.Items.Add($item)
}
$comboSaveClass.SelectedItem = $script:SelectedSaveClass
$comboSaveClass.Add_SelectedIndexChanged({
    Update-SaveUi
    Set-Status "save label -> $script:SelectedSaveClass"
})
$panelTop.Controls.Add($comboSaveClass)

$labelStatus = New-Object System.Windows.Forms.Label
$labelStatus.Left = 924
$labelStatus.Top = 15
$labelStatus.Width = 260
$labelStatus.Font = $buttonFont
$labelStatus.ForeColor = [System.Drawing.Color]::White
$panelTop.Controls.Add($labelStatus)

$pictureBox = New-Object System.Windows.Forms.PictureBox
$pictureBox.Dock = "Fill"
$pictureBox.BackColor = [System.Drawing.Color]::Black
$pictureBox.SizeMode = [System.Windows.Forms.PictureBoxSizeMode]::Zoom
$form.Controls.Add($pictureBox)

$panelRight = New-Object System.Windows.Forms.Panel
$panelRight.Dock = "Right"
$panelRight.Width = 340
$panelRight.BackColor = [System.Drawing.Color]::White
$panelRight.AutoScroll = $true
$form.Controls.Add($panelRight)

$labelTitle = New-Object System.Windows.Forms.Label
$labelTitle.Left = 12
$labelTitle.Top = 16
$labelTitle.Width = 300
$labelTitle.Font = New-Object System.Drawing.Font("Segoe UI", 17, [System.Drawing.FontStyle]::Bold)
$labelTitle.Text = "AI RESULT"
$panelRight.Controls.Add($labelTitle)

$labelClassValue = New-Object System.Windows.Forms.Label
$labelClassValue.Left = 12
$labelClassValue.Top = 52
$labelClassValue.Width = 300
$labelClassValue.Height = 34
$labelClassValue.TextAlign = [System.Drawing.ContentAlignment]::MiddleCenter
$labelClassValue.Font = New-Object System.Drawing.Font("Segoe UI", 12, [System.Drawing.FontStyle]::Bold)
$labelClassValue.ForeColor = [System.Drawing.Color]::White
$panelRight.Controls.Add($labelClassValue)

$labelClass = New-Object System.Windows.Forms.Label
$labelClass.Left = 12
$labelClass.Top = 100
$labelClass.Width = 300
$labelClass.Font = $valueFont
$panelRight.Controls.Add($labelClass)

$labelRawClass = New-Object System.Windows.Forms.Label
$labelRawClass.Left = 12
$labelRawClass.Top = 126
$labelRawClass.Width = 300
$labelRawClass.Font = $valueFont
$panelRight.Controls.Add($labelRawClass)

$labelConfidence = New-Object System.Windows.Forms.Label
$labelConfidence.Left = 12
$labelConfidence.Top = 156
$labelConfidence.Width = 300
$labelConfidence.Font = $valueFont
$panelRight.Controls.Add($labelConfidence)

$progressConfidence = New-Object System.Windows.Forms.ProgressBar
$progressConfidence.Left = 12
$progressConfidence.Top = 182
$progressConfidence.Width = 300
$progressConfidence.Minimum = 0
$progressConfidence.Maximum = 1000
$panelRight.Controls.Add($progressConfidence)

$labelRegression = New-Object System.Windows.Forms.Label
$labelRegression.Left = 12
$labelRegression.Top = 216
$labelRegression.Width = 300
$labelRegression.Font = $valueFont
$panelRight.Controls.Add($labelRegression)

$progressWater = New-Object System.Windows.Forms.ProgressBar
$progressWater.Left = 12
$progressWater.Top = 242
$progressWater.Width = 300
$progressWater.Minimum = 0
$progressWater.Maximum = 1000
$panelRight.Controls.Add($progressWater)

$labelPipeline = New-Object System.Windows.Forms.Label
$labelPipeline.Left = 12
$labelPipeline.Top = 278
$labelPipeline.Width = 300
$labelPipeline.Font = $valueFont
$panelRight.Controls.Add($labelPipeline)

$labelInfer = New-Object System.Windows.Forms.Label
$labelInfer.Left = 12
$labelInfer.Top = 304
$labelInfer.Width = 300
$labelInfer.Font = $valueFont
$panelRight.Controls.Add($labelInfer)

$labelFps = New-Object System.Windows.Forms.Label
$labelFps.Left = 12
$labelFps.Top = 330
$labelFps.Width = 300
$labelFps.Font = $valueFont
$panelRight.Controls.Add($labelFps)

$labelFrame = New-Object System.Windows.Forms.Label
$labelFrame.Left = 12
$labelFrame.Top = 356
$labelFrame.Width = 300
$labelFrame.Font = $valueFont
$panelRight.Controls.Add($labelFrame)

$labelJpeg = New-Object System.Windows.Forms.Label
$labelJpeg.Left = 12
$labelJpeg.Top = 382
$labelJpeg.Width = 300
$labelJpeg.Font = $valueFont
$panelRight.Controls.Add($labelJpeg)

$labelLogits = New-Object System.Windows.Forms.Label
$labelLogits.Left = 12
$labelLogits.Top = 416
$labelLogits.Width = 300
$labelLogits.Height = 50
$labelLogits.Font = $monoFont
$panelRight.Controls.Add($labelLogits)

$labelDiag = New-Object System.Windows.Forms.Label
$labelDiag.Left = 12
$labelDiag.Top = 476
$labelDiag.Width = 300
$labelDiag.Height = 20
$labelDiag.Font = $valueFont
$labelDiag.Text = "Diag"
$panelRight.Controls.Add($labelDiag)

$textDiag = New-Object System.Windows.Forms.TextBox
$textDiag.Left = 12
$textDiag.Top = 500
$textDiag.Width = 300
$textDiag.Height = 96
$textDiag.Multiline = $true
$textDiag.ReadOnly = $true
$textDiag.ScrollBars = "Vertical"
$textDiag.Font = $monoFont
$panelRight.Controls.Add($textDiag)

$labelSaveDir = New-Object System.Windows.Forms.Label
$labelSaveDir.Left = 12
$labelSaveDir.Top = 606
$labelSaveDir.Width = 300
$labelSaveDir.Height = 20
$labelSaveDir.Font = $valueFont
$labelSaveDir.Text = "Save Root"
$panelRight.Controls.Add($labelSaveDir)

$textSaveRoot = New-Object System.Windows.Forms.TextBox
$textSaveRoot.Left = 12
$textSaveRoot.Top = 630
$textSaveRoot.Width = 300
$textSaveRoot.Height = 24
$textSaveRoot.Multiline = $false
$textSaveRoot.ReadOnly = $true
$textSaveRoot.ScrollBars = "Horizontal"
$textSaveRoot.Font = $monoFont
$panelRight.Controls.Add($textSaveRoot)

$buttonRootPick = New-Object System.Windows.Forms.Button
$buttonRootPick.Left = 12
$buttonRootPick.Top = 660
$buttonRootPick.Width = 96
$buttonRootPick.Height = 28
$buttonRootPick.Font = $buttonFont
$buttonRootPick.Text = "Choose Root"
$buttonRootPick.FlatStyle = [System.Windows.Forms.FlatStyle]::Standard
$buttonRootPick.Add_Click({
    $dialog = New-Object System.Windows.Forms.FolderBrowserDialog
    $dialog.SelectedPath = $script:SaveDir
    $dialog.Description = "Select dataset root folder"
    if ($dialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
        $script:SaveDir = $dialog.SelectedPath
        Ensure-SaveDir
        Update-SaveUi
        Set-Status "save root -> $script:SaveDir"
    }
    $dialog.Dispose()
})
$panelRight.Controls.Add($buttonRootPick)

$labelTargetFolder = New-Object System.Windows.Forms.Label
$labelTargetFolder.Left = 118
$labelTargetFolder.Top = 666
$labelTargetFolder.Width = 194
$labelTargetFolder.Height = 20
$labelTargetFolder.Font = $valueFont
$labelTargetFolder.Text = "Folder: label_0_no_cup"
$panelRight.Controls.Add($labelTargetFolder)

$labelSaveCount = New-Object System.Windows.Forms.Label
$labelSaveCount.Left = 12
$labelSaveCount.Top = 700
$labelSaveCount.Width = 300
$labelSaveCount.Font = $valueFont
$labelSaveCount.Text = "Photos: 0"
$panelRight.Controls.Add($labelSaveCount)

$timer = New-Object System.Windows.Forms.Timer
$timer.Interval = 30
$timer.Add_Tick({
    if (($script:SerialPort -eq $null) -or (-not $script:SerialPort.IsOpen)) {
        return
    }

    try {
        $available = $script:SerialPort.BytesToRead
        if ($available -gt 0) {
            $chunk = New-Object byte[] $available
            $count = $script:SerialPort.Read($chunk, 0, $available)
            if ($count -gt 0) {
                $script:TotalBytesReceived += $count
                if ($count -eq $chunk.Length) {
                    $script:Buffer.AddRange($chunk)
                } else {
                    $readBytes = New-Object byte[] $count
                    [System.Array]::Copy($chunk, $readBytes, $count)
                    $script:Buffer.AddRange($readBytes)
                }
            }
        }

        $latestFrame = $null
        while ($true) {
            $frame = Try-ExtractFrame
            if ($frame -eq $null) {
                break
            }
            $latestFrame = $frame
        }

        if ($latestFrame -ne $null) {
            $nowTick = [System.Diagnostics.Stopwatch]::GetTimestamp()
            if ($script:LastFrameTick -ne $null) {
                $deltaMs = ([double]($nowTick - $script:LastFrameTick) * 1000.0) / [System.Diagnostics.Stopwatch]::Frequency
                if ($deltaMs -gt 0.0) {
                    $frameDelta = 1.0
                    if (($script:LastFrameId -ne $null) -and ($latestFrame.FrameId -gt $script:LastFrameId)) {
                        $frameDelta = [double]($latestFrame.FrameId - $script:LastFrameId)
                    }
                    $instantFps = ($frameDelta * 1000.0) / $deltaMs
                    if ($script:DisplayFps -le 0.0) {
                        $script:DisplayFps = $instantFps
                    } else {
                        $script:DisplayFps = ($script:DisplayFps * 0.7) + ($instantFps * 0.3)
                    }
                }
            }
            $script:LastFrameTick = $nowTick
            $script:LastFrameId = $latestFrame.FrameId
            Update-Viewer -Frame $latestFrame
            if ($script:AutoSaveEnabled) {
                Save-FrameArtifacts -Frame $latestFrame
            }
            $textDiag.Text = "len=$($latestFrame.JpegLen)`r`nhead=$($latestFrame.JpegHead)`r`ntail=$($latestFrame.JpegTail)`r`nend=$($latestFrame.TailMagic)`r`nfail=$([Environment]::NewLine)$script:LastFailedJpegPath"
        } else {
            $textDiag.Text = "rx=$script:TotalBytesReceived bytes`r`nbuffer=$($script:Buffer.Count) bytes`r`nwaiting for AIV1 frame`r`n--- device output ---`r`n$script:LastTextLog"
            $textDiag.SelectionStart = $textDiag.TextLength
            $textDiag.ScrollToCaret()
        }
    } catch {
        Disconnect-Serial
        Set-Status "serial read failed: $($_.Exception.Message)"
    }
})

$form.Add_Shown({
    Refresh-PortList
    Ensure-SaveDir
    Update-SaveUi
    Set-Status "ready"
    if ($comboPorts.SelectedItem) {
        $Port = [string]$comboPorts.SelectedItem
        Connect-Serial
    }
})

$form.Add_FormClosing({
    $timer.Stop()
    Disconnect-Serial
    if ($pictureBox.Image -ne $null) {
        $pictureBox.Image.Dispose()
        $pictureBox.Image = $null
    }
})

$timer.Start()
[void]$form.ShowDialog()
