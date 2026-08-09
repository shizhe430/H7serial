param(
    [string]$Port = 'COM28',
    [int]$Baud = 921600,
    [switch]$AutoOpen
)

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

$script:HeaderSize = 48
$script:Magic = [byte[]](0x46, 0x41, 0x56, 0x31)
$script:Tail = [byte[]](0x46, 0x41, 0x45, 0x31)
$script:Buffer = New-Object 'System.Collections.Generic.List[byte]'
$script:Serial = $null
$script:AutoOpenRequested = [bool]$AutoOpen
$script:DiagnosticText = ''
$script:CurrentBitmap = $null
$script:LastFrameTick = $null
$script:DisplayFps = 0.0
$script:RxBytes = 0L
$script:FrameCount = 0L
$script:EnrollmentPending = $false
$script:EnrollmentRequestAfterFrame = 0
$script:EnrollmentId = [uint16]1
$script:EnrollmentAccepted = 0
$script:ReferenceReadyLatched = $false
$script:LastFrameId = 0
$script:MatchCount = 0
$script:NoMatchCount = 0
$script:BestSimilarity = -1.0

function Get-U16 {
    param([byte[]]$Bytes, [int]$Offset)
    return [uint16]([int]$Bytes[$Offset] -bor ([int]$Bytes[$Offset + 1] -shl 8))
}

function Get-I16 {
    param([byte[]]$Bytes, [int]$Offset)
    $value = [int](Get-U16 $Bytes $Offset)
    if ($value -ge 32768) { $value -= 65536 }
    return $value
}

function Get-U32 {
    param([byte[]]$Bytes, [int]$Offset)
    return [BitConverter]::ToUInt32($Bytes, $Offset)
}

function Get-I32 {
    param([byte[]]$Bytes, [int]$Offset)
    return [BitConverter]::ToInt32($Bytes, $Offset)
}

function Find-MagicIndex {
    for ($i = 0; $i -le ($script:Buffer.Count - $script:Magic.Length); $i++) {
        $match = $true
        for ($j = 0; $j -lt $script:Magic.Length; $j++) {
            if ($script:Buffer[$i + $j] -ne $script:Magic[$j]) {
                $match = $false
                break
            }
        }
        if ($match) { return $i }
    }
    return -1
}

function Add-LogLine {
    param([string]$Line)
    if ([string]::IsNullOrWhiteSpace($Line)) { return }
    $stamp = [DateTime]::Now.ToString('HH:mm:ss.fff')
    $textLog.AppendText("[$stamp] $Line`r`n")
    if ($textLog.Lines.Count -gt 250) {
        $cut = $textLog.GetFirstCharIndexFromLine(50)
        if ($cut -gt 0) {
            $textLog.Select(0, $cut)
            $textLog.SelectedText = ''
        }
    }
    $textLog.SelectionStart = $textLog.TextLength
    $textLog.ScrollToCaret()
}

function Add-DiagnosticBytes {
    param([byte[]]$Bytes)
    if (($null -eq $Bytes) -or ($Bytes.Length -eq 0)) { return }
    $text = [Text.Encoding]::ASCII.GetString($Bytes)
    $text = [Text.RegularExpressions.Regex]::Replace($text, '[^\x09\x0A\x0D\x20-\x7E]', '.')
    $script:DiagnosticText += $text
    $parts = $script:DiagnosticText -split "`r?`n", -1
    $script:DiagnosticText = $parts[$parts.Count - 1]
    for ($i = 0; $i -lt ($parts.Count - 1); $i++) {
        $line = $parts[$i]
        if ($line -match '^\[OPENMV\] enroll begin id=([0-9]+)') {
            $labelResult.Text = ('ENROLL ID={0}' -f $Matches[1]) + [Environment]::NewLine + 'BOARD ACK; SHOW FRONTAL FACE'
            $labelResult.ForeColor = [Drawing.Color]::DarkOrange
        } elseif ($line -match '^\[OPENMV\] enroll id=([0-9]+) accepted=([0-9]+)/5 remaining=([0-9]+)') {
            $labelResult.Text = ('ENROLL ID={0}' -f $Matches[1]) + [Environment]::NewLine +
                                ('ACCEPTED {0}/5; REMAINING {1}' -f $Matches[2], $Matches[3])
            $labelResult.ForeColor = [Drawing.Color]::DarkOrange
        } elseif ($line -match '^\[OPENMV\] enroll reject id=([0-9]+)') {
            $script:EnrollmentPending = $false
            $buttonEnroll.Enabled = $true
            $buttonEnroll.Text = 'Enroll'
            $labelResult.Text = ('ENROLL ID={0} REJECTED' -f $Matches[1])
            $labelResult.ForeColor = [Drawing.Color]::Crimson
        }
        if ($line -match '^\[[A-Z0-9_:-]+\]') { Add-LogLine $line }
    }
    if ($script:DiagnosticText.Length -gt 4096) {
        $script:DiagnosticText = $script:DiagnosticText.Substring($script:DiagnosticText.Length - 1024)
    }
}

function Set-ConnectionState {
    param([string]$Text, [Drawing.Color]$Color)
    $labelConnection.Text = $Text
    $labelConnection.ForeColor = $Color
}

function Set-FrameResult {
    param($Frame)

    $now = [DateTime]::Now
    if ($null -ne $script:LastFrameTick) {
        $seconds = ($now - $script:LastFrameTick).TotalSeconds
        if ($seconds -gt 0.0) { $script:DisplayFps = 1.0 / $seconds }
    }
    $script:LastFrameTick = $now
    $script:FrameCount++
    $script:LastFrameId = [uint32]$Frame.FrameId

    $stream = New-Object IO.MemoryStream(,$Frame.Jpeg)
    try {
        $source = [Drawing.Image]::FromStream($stream)
        try {
            $bitmap = New-Object Drawing.Bitmap($source.Width, $source.Height, [Drawing.Imaging.PixelFormat]::Format24bppRgb)
            $graphics = [Drawing.Graphics]::FromImage($bitmap)
            try {
                $graphics.DrawImage($source, 0, 0, $source.Width, $source.Height)
                $hasBox = ($Frame.X2 -gt $Frame.X1) -and ($Frame.Y2 -gt $Frame.Y1)
                if ($hasBox) {
                    $boxColor = if ($Frame.MatchValid) { [Drawing.Color]::LimeGreen } else { [Drawing.Color]::DarkOrange }
                    $pen = New-Object Drawing.Pen($boxColor, 3)
                    $font = New-Object Drawing.Font('Segoe UI', 12, [Drawing.FontStyle]::Bold)
                    $brush = New-Object Drawing.SolidBrush($boxColor)
                    $back = New-Object Drawing.SolidBrush([Drawing.Color]::FromArgb(180, 0, 0, 0))
                    try {
                        $x1 = [Math]::Max(0, [Math]::Min($bitmap.Width - 1, $Frame.X1))
                        $y1 = [Math]::Max(0, [Math]::Min($bitmap.Height - 1, $Frame.Y1))
                        $x2 = [Math]::Max($x1 + 1, [Math]::Min($bitmap.Width - 1, $Frame.X2))
                        $y2 = [Math]::Max($y1 + 1, [Math]::Min($bitmap.Height - 1, $Frame.Y2))
                        $graphics.DrawRectangle($pen, $x1, $y1, $x2 - $x1, $y2 - $y1)
                        $caption = if ($Frame.MatchValid) {
                            'ID={0} MATCH' -f $Frame.MatchedId
                        } elseif ($Frame.ReferenceReady -and $Frame.EmbeddingValid) {
                            'FACE  NO MATCH'
                        } else {
                            'FACE'
                        }
                        $size = $graphics.MeasureString($caption, $font)
                        $textY = [Math]::Max(0, $y1 - [int]$size.Height - 2)
                        $graphics.FillRectangle($back, $x1, $textY, [int]$size.Width + 8, [int]$size.Height + 2)
                        $graphics.DrawString($caption, $font, $brush, $x1 + 4, $textY)
                    } finally {
                        $pen.Dispose()
                        $font.Dispose()
                        $brush.Dispose()
                        $back.Dispose()
                    }
                }
            } finally {
                $graphics.Dispose()
            }
        } finally {
            $source.Dispose()
        }
    } finally {
        $stream.Dispose()
    }

    $old = $script:CurrentBitmap
    $script:CurrentBitmap = $bitmap
    $picture.Image = $script:CurrentBitmap
    if ($null -ne $old) { $old.Dispose() }

    $labelFrame.Text = [string]$Frame.FrameId
    $labelCamera.Text = 'Camera ' + $Frame.Camera
    $labelJpeg.Text = ('{0:N0} B' -f $Frame.Jpeg.Length)
    $labelFps.Text = ('{0:F2} fps' -f $script:DisplayFps)
    $labelScore.Text = ('{0:F3}' -f $Frame.Score)
    $labelBox.Text = ('{0},{1} -> {2},{3}' -f $Frame.X1, $Frame.Y1, $Frame.X2, $Frame.Y2)
    $labelDetTime.Text = $Frame.DetectionMs.ToString() + ' ms'
    $labelIdTime.Text = if ($Frame.EmbeddingValid) { $Frame.IdentityMs.ToString() + ' ms' } else { '-' }
    $labelTotalTime.Text = $Frame.TotalMs.ToString() + ' ms'
    $labelNorm.Text = if ($Frame.EmbeddingValid) { '{0:F3}' -f $Frame.EmbeddingNorm } else { '-' }
    if ($Frame.ReferenceReady) {
        $script:ReferenceReadyLatched = $true
    }
    $labelReference.Text = if ($script:ReferenceReadyLatched) { 'DATABASE READY' } else { 'EMPTY' }
    $labelSimilarity.Text = if ($Frame.ReferenceReady -and $Frame.DetectionValid) { '{0:F3}' -f $Frame.Similarity } else { '-' }
    $labelStatusCode.Text = [string]$Frame.Status

    # Only a complete SFace result changes the decision banner. Detection-only
    # and no-face frames must not erase a result the operator is reading.
    if ($script:EnrollmentPending -and
        ($Frame.FrameId -gt $script:EnrollmentRequestAfterFrame) -and
        $Frame.EmbeddingValid) {
        if (($Frame.Status -eq 0) -and $Frame.ReferenceReady -and
            $Frame.MatchValid -and ($Frame.MatchedId -eq $script:EnrollmentId)) {
            $script:EnrollmentPending = $false
            $buttonEnroll.Enabled = $true
            $buttonEnroll.Text = 'Enroll'
            $labelResult.Text = ('ID={0} SAVED' -f $script:EnrollmentId) +
                                [Environment]::NewLine + ('MATCH ID={0}' -f $script:EnrollmentId)
            $labelResult.ForeColor = [Drawing.Color]::ForestGreen
            Add-LogLine ('enrollment confirmed frame={0} id={1} similarity={2:F3}' -f $Frame.FrameId, $script:EnrollmentId, $Frame.Similarity)
        } elseif ($Frame.Status -ne 0) {
            $script:EnrollmentPending = $false
            $buttonEnroll.Enabled = $true
            $buttonEnroll.Text = 'Enroll'
            $labelResult.Text = ('ENROLL ID={0}' -f $script:EnrollmentId) + [Environment]::NewLine +
                                ('FAILED STATUS={0}' -f $Frame.Status)
            $labelResult.ForeColor = [Drawing.Color]::Crimson
            Add-LogLine ('enrollment failed frame={0} id={1} status={2}' -f $Frame.FrameId, $script:EnrollmentId, $Frame.Status)
        } elseif ($Frame.BackendId -eq 2) {
            $script:EnrollmentAccepted = [Math]::Min(4, $script:EnrollmentAccepted + 1)
            $labelResult.Text = ('ENROLL ID={0}' -f $script:EnrollmentId) + [Environment]::NewLine +
                                ('ACCEPTED {0}/5; VARY FRONTAL POSE' -f $script:EnrollmentAccepted)
            $labelResult.ForeColor = [Drawing.Color]::DarkOrange
        } else {
            $script:EnrollmentPending = $false
            $buttonEnroll.Enabled = $true
            $buttonEnroll.Text = 'Enroll'
            $labelResult.Text = ('ENROLL ID={0}' -f $script:EnrollmentId) + [Environment]::NewLine + 'CHECK LOG FOR SAVE ERROR'
            $labelResult.ForeColor = [Drawing.Color]::Crimson
            Add-LogLine ('enrollment failed frame={0} id={1}' -f $Frame.FrameId, $script:EnrollmentId)
        }
    } elseif ($Frame.EmbeddingValid) {
        if ($Frame.Status -ne 0) {
            $labelResult.Text = 'INFERENCE ERROR'
            $labelResult.ForeColor = [Drawing.Color]::Crimson
            Add-LogLine ('decision frame={0} status={1}' -f $Frame.FrameId, $Frame.Status)
        } elseif ($Frame.MatchValid) {
            $labelResult.Text = 'DATABASE READY' + [Environment]::NewLine + 'MATCH ID=' + $Frame.MatchedId
            $labelResult.ForeColor = [Drawing.Color]::ForestGreen
            $script:MatchCount++
            if ($Frame.Similarity -gt $script:BestSimilarity) { $script:BestSimilarity = $Frame.Similarity }
            Add-LogLine ('decision frame={0} MATCH id={1} distance={2:F3} similarity={3:F3}' -f $Frame.FrameId, $Frame.MatchedId, $Frame.EmbeddingNorm, $Frame.Similarity)
        } elseif ($Frame.ReferenceReady) {
            $labelResult.Text = 'DATABASE READY' + [Environment]::NewLine + 'NO MATCH'
            $labelResult.ForeColor = [Drawing.Color]::Crimson
            $script:NoMatchCount++
            if ($Frame.Similarity -gt $script:BestSimilarity) { $script:BestSimilarity = $Frame.Similarity }
            Add-LogLine ('decision frame={0} NO MATCH distance={1:F3} similarity={2:F3}' -f $Frame.FrameId, $Frame.EmbeddingNorm, $Frame.Similarity)
        } else {
            $labelResult.Text = 'FACE + FEATURE OK'
            $labelResult.ForeColor = [Drawing.Color]::ForestGreen
            Add-LogLine ('decision frame={0} reference=EMPTY' -f $Frame.FrameId)
        }
    }
    $labelMatchCount.Text = [string]$script:MatchCount
    $labelNoMatchCount.Text = [string]$script:NoMatchCount
    $labelBestSimilarity.Text = if ($script:BestSimilarity -ge 0.0) { '{0:F3}' -f $script:BestSimilarity } else { '-' }
    $labelTraffic.Text = ('frames={0}  rx={1:N0} B' -f $script:FrameCount, $script:RxBytes)
}

function Parse-Frames {
    while ($true) {
        $magicIndex = Find-MagicIndex
        if ($magicIndex -lt 0) {
            if ($script:Buffer.Count -gt 3) {
                $count = $script:Buffer.Count - 3
                $diag = $script:Buffer.GetRange(0, $count).ToArray()
                $script:Buffer.RemoveRange(0, $count)
                Add-DiagnosticBytes $diag
            }
            return
        }

        if ($magicIndex -gt 0) {
            $diag = $script:Buffer.GetRange(0, $magicIndex).ToArray()
            $script:Buffer.RemoveRange(0, $magicIndex)
            Add-DiagnosticBytes $diag
        }
        if ($script:Buffer.Count -lt $script:HeaderSize) { return }

        $header = $script:Buffer.GetRange(0, $script:HeaderSize).ToArray()
        $jpegLen = [int](Get-U32 $header 12)
        if (($header[4] -ne 1) -or ($jpegLen -lt 4) -or ($jpegLen -gt 65535)) {
            $script:Buffer.RemoveAt(0)
            continue
        }

        $packetSize = $script:HeaderSize + $jpegLen + $script:Tail.Length
        if ($script:Buffer.Count -lt $packetSize) { return }
        $tailOffset = $script:HeaderSize + $jpegLen
        $tailOk = $true
        for ($i = 0; $i -lt $script:Tail.Length; $i++) {
            if ($script:Buffer[$tailOffset + $i] -ne $script:Tail[$i]) {
                $tailOk = $false
                break
            }
        }
        $jpeg = $script:Buffer.GetRange($script:HeaderSize, $jpegLen).ToArray()
        $jpegOk = ($jpeg[0] -eq 0xFF) -and ($jpeg[1] -eq 0xD8) -and
                  ($jpeg[$jpeg.Length - 2] -eq 0xFF) -and ($jpeg[$jpeg.Length - 1] -eq 0xD9)
        if ((-not $tailOk) -or (-not $jpegOk)) {
            Add-LogLine ('packet resync: tail=' + $tailOk + ' jpeg=' + $jpegOk)
            $script:Buffer.RemoveAt(0)
            continue
        }

        $flags = [int]$header[6]
        $frame = [pscustomobject]@{
            Camera         = [int]$header[5]
            Status         = [int]$header[7]
            FrameId        = [uint32](Get-U32 $header 8)
            Score          = ([double](Get-U16 $header 16)) / 1000.0
            DetectionValid = (($flags -band 0x01) -ne 0)
            EmbeddingValid = (($flags -band 0x02) -ne 0)
            ReferenceReady = (($flags -band 0x04) -ne 0)
            MatchValid     = (($flags -band 0x08) -ne 0)
            Similarity     = ([double](Get-U16 $header 44)) / 1000.0
            BackendId      = [byte]$header[26]
            MatchedId      = [uint16](Get-U16 $header 46)
            X1             = Get-I16 $header 18
            Y1             = Get-I16 $header 20
            X2             = Get-I16 $header 22
            Y2             = Get-I16 $header 24
            DetectionMs    = [uint32](Get-U32 $header 28)
            IdentityMs     = [uint32](Get-U32 $header 32)
            TotalMs        = [uint32](Get-U32 $header 36)
            EmbeddingNorm  = ([double](Get-I32 $header 40)) / 1000.0
            Jpeg           = $jpeg
        }
        $script:Buffer.RemoveRange(0, $packetSize)
        try {
            Set-FrameResult $frame
        } catch {
            Add-LogLine ('frame display error: ' + $_.Exception.Message)
        }
    }
}

function Open-Serial {
    if ($null -ne $script:Serial -and $script:Serial.IsOpen) { return }
    try {
        $script:Serial = New-Object IO.Ports.SerialPort(
            $textPort.Text.Trim(),
            [int]$textBaud.Text,
            [IO.Ports.Parity]::None,
            8,
            [IO.Ports.StopBits]::One)
        $script:Serial.Handshake = [IO.Ports.Handshake]::None
        $script:Serial.DtrEnable = $false
        $script:Serial.RtsEnable = $false
        $script:Serial.ReadTimeout = 5
        $script:Serial.WriteTimeout = 1000
        $script:Serial.ReadBufferSize = 1048576
        $script:Serial.Open()
        $script:Buffer.Clear()
        Set-ConnectionState ('OPEN ' + $textPort.Text.Trim()) ([Drawing.Color]::ForestGreen)
        $buttonConnect.Text = 'Close'
        Add-LogLine ('serial opened ' + $textPort.Text.Trim() + ' @ ' + $textBaud.Text)
    } catch {
        Set-ConnectionState 'OPEN FAILED' ([Drawing.Color]::Crimson)
        [Windows.Forms.MessageBox]::Show($_.Exception.Message, 'Face AI visual viewer') | Out-Null
    }
}

function Close-Serial {
    if ($null -ne $script:Serial) {
        try {
            if ($script:Serial.IsOpen) { $script:Serial.Close() }
            $script:Serial.Dispose()
        } catch { }
        $script:Serial = $null
    }
    $buttonConnect.Text = 'Open'
    Set-ConnectionState 'CLOSED' ([Drawing.Color]::DimGray)
}

function Send-FaceCommand {
    param([string]$Command)
    if ($null -eq $script:Serial -or (-not $script:Serial.IsOpen)) {
        [Windows.Forms.MessageBox]::Show('Open COM28 first.', 'Face AI visual viewer') | Out-Null
        return
    }
    try {
        $script:Serial.WriteLine($Command)
        Add-LogLine ('command sent: ' + $Command)
        if ($Command -match '^ENROLL ([0-9]+)$') {
            $script:EnrollmentId = [uint16]$Matches[1]
            $script:EnrollmentPending = $true
            $script:EnrollmentRequestAfterFrame = $script:LastFrameId
            $script:EnrollmentAccepted = 0
            $buttonEnroll.Enabled = $false
            $buttonEnroll.Text = 'Waiting...'
            $labelResult.Text = ('ENROLL ID={0}' -f $script:EnrollmentId) + [Environment]::NewLine + 'WAITING FOR FACE'
            $labelResult.ForeColor = [Drawing.Color]::DarkOrange
        } elseif ($Command -eq 'CLEAR') {
            $script:EnrollmentPending = $false
            $script:ReferenceReadyLatched = $false
            $script:MatchCount = 0
            $script:NoMatchCount = 0
            $script:BestSimilarity = -1.0
            $buttonEnroll.Enabled = $true
            $buttonEnroll.Text = 'Enroll'
            $labelReference.Text = 'EMPTY'
            $labelSimilarity.Text = '-'
            $labelResult.Text = 'REFERENCE EMPTY'
            $labelResult.ForeColor = [Drawing.Color]::DimGray
        }
    } catch {
        Add-LogLine ('command error: ' + $_.Exception.Message)
    }
}

function New-ValueRow {
    param([Windows.Forms.TableLayoutPanel]$Parent, [string]$Title)
    $row = $Parent.RowCount
    $Parent.RowCount++
    [void]$Parent.RowStyles.Add((New-Object Windows.Forms.RowStyle([Windows.Forms.SizeType]::Absolute, 32)))
    $name = New-Object Windows.Forms.Label
    $name.Text = $Title
    $name.Dock = 'Fill'
    $name.TextAlign = 'MiddleLeft'
    $name.ForeColor = [Drawing.Color]::DimGray
    $value = New-Object Windows.Forms.Label
    $value.Text = '-'
    $value.Dock = 'Fill'
    $value.TextAlign = 'MiddleRight'
    $value.Font = New-Object Drawing.Font('Segoe UI', 9, [Drawing.FontStyle]::Bold)
    [void]$Parent.Controls.Add($name, 0, $row)
    [void]$Parent.Controls.Add($value, 1, $row)
    return $value
}

$form = New-Object Windows.Forms.Form
$form.Text = 'H7 Face AI Visual Viewer'
$form.StartPosition = 'CenterScreen'
$form.Size = New-Object Drawing.Size(1220, 820)
$form.MinimumSize = New-Object Drawing.Size(980, 680)
$form.BackColor = [Drawing.Color]::WhiteSmoke

$root = New-Object Windows.Forms.TableLayoutPanel
$root.Dock = 'Fill'
$root.ColumnCount = 1
$root.RowCount = 3
$root.RowStyles.Add((New-Object Windows.Forms.RowStyle([Windows.Forms.SizeType]::Absolute, 52)))
$root.RowStyles.Add((New-Object Windows.Forms.RowStyle([Windows.Forms.SizeType]::Percent, 78)))
$root.RowStyles.Add((New-Object Windows.Forms.RowStyle([Windows.Forms.SizeType]::Percent, 22)))
$form.Controls.Add($root)

$top = New-Object Windows.Forms.FlowLayoutPanel
$top.Dock = 'Fill'
$top.FlowDirection = 'LeftToRight'
$top.Padding = New-Object Windows.Forms.Padding(10, 9, 10, 7)
$top.WrapContents = $false
$top.BackColor = [Drawing.Color]::Gainsboro
$root.Controls.Add($top, 0, 0)

$top.Controls.Add((New-Object Windows.Forms.Label -Property @{Text='Port';AutoSize=$true;Margin=(New-Object Windows.Forms.Padding(3,7,3,0))}))
$textPort = New-Object Windows.Forms.TextBox
$textPort.Text = $Port
$textPort.Width = 72
$top.Controls.Add($textPort)
$top.Controls.Add((New-Object Windows.Forms.Label -Property @{Text='Baud';AutoSize=$true;Margin=(New-Object Windows.Forms.Padding(12,7,3,0))}))
$textBaud = New-Object Windows.Forms.TextBox
$textBaud.Text = $Baud
$textBaud.Width = 88
$top.Controls.Add($textBaud)
$buttonConnect = New-Object Windows.Forms.Button
$buttonConnect.Text = 'Open'
$buttonConnect.Width = 82
$top.Controls.Add($buttonConnect)
$buttonSave = New-Object Windows.Forms.Button
$buttonSave.Text = 'Save frame'
$buttonSave.Width = 92
$top.Controls.Add($buttonSave)
$top.Controls.Add((New-Object Windows.Forms.Label -Property @{Text='ID';AutoSize=$true;Margin=(New-Object Windows.Forms.Padding(10,7,3,0))}))
$numericId = New-Object Windows.Forms.NumericUpDown
$numericId.Minimum = 1
$numericId.Maximum = 65535
$numericId.Value = 1
$numericId.Width = 64
$top.Controls.Add($numericId)
$buttonEnroll = New-Object Windows.Forms.Button
$buttonEnroll.Text = 'Enroll'
$buttonEnroll.Width = 72
$top.Controls.Add($buttonEnroll)
$buttonDeleteEnroll = New-Object Windows.Forms.Button
$buttonDeleteEnroll.Text = 'Delete'
$buttonDeleteEnroll.Width = 68
$top.Controls.Add($buttonDeleteEnroll)
$buttonListEnroll = New-Object Windows.Forms.Button
$buttonListEnroll.Text = 'List'
$buttonListEnroll.Width = 58
$top.Controls.Add($buttonListEnroll)
$buttonClearEnroll = New-Object Windows.Forms.Button
$buttonClearEnroll.Text = 'Clear all'
$buttonClearEnroll.Width = 72
$top.Controls.Add($buttonClearEnroll)
$labelConnection = New-Object Windows.Forms.Label
$labelConnection.Text = 'CLOSED'
$labelConnection.AutoSize = $true
$labelConnection.Font = New-Object Drawing.Font('Segoe UI', 10, [Drawing.FontStyle]::Bold)
$labelConnection.Margin = New-Object Windows.Forms.Padding(18, 7, 3, 0)
$top.Controls.Add($labelConnection)
$labelTraffic = New-Object Windows.Forms.Label
$labelTraffic.Text = 'frames=0  rx=0 B'
$labelTraffic.AutoSize = $true
$labelTraffic.Margin = New-Object Windows.Forms.Padding(22, 7, 3, 0)
$top.Controls.Add($labelTraffic)

$split = New-Object Windows.Forms.SplitContainer
$split.Dock = 'Fill'
$split.Orientation = 'Vertical'
$split.SplitterDistance = 820
$split.Panel1MinSize = 600
$split.Panel2MinSize = 270
$root.Controls.Add($split, 0, 1)

$picture = New-Object Windows.Forms.PictureBox
$picture.Dock = 'Fill'
$picture.SizeMode = 'Zoom'
$picture.BackColor = [Drawing.Color]::FromArgb(24, 26, 28)
$split.Panel1.Padding = New-Object Windows.Forms.Padding(10)
$split.Panel1.Controls.Add($picture)

$metrics = New-Object Windows.Forms.TableLayoutPanel
$metrics.Dock = 'Fill'
$metrics.Padding = New-Object Windows.Forms.Padding(12)
$metrics.ColumnCount = 2
$metrics.RowCount = 1
$metrics.ColumnStyles.Add((New-Object Windows.Forms.ColumnStyle([Windows.Forms.SizeType]::Percent, 48)))
$metrics.ColumnStyles.Add((New-Object Windows.Forms.ColumnStyle([Windows.Forms.SizeType]::Percent, 52)))
$split.Panel2.Controls.Add($metrics)

$labelResult = New-Object Windows.Forms.Label
$labelResult.Text = 'WAITING FOR FAV1'
$labelResult.Dock = 'Fill'
$labelResult.TextAlign = 'MiddleCenter'
$labelResult.Font = New-Object Drawing.Font('Segoe UI', 15, [Drawing.FontStyle]::Bold)
$labelResult.ForeColor = [Drawing.Color]::DimGray
$metrics.Controls.Add($labelResult, 0, 0)
$metrics.SetColumnSpan($labelResult, 2)
$metrics.RowStyles.Add((New-Object Windows.Forms.RowStyle([Windows.Forms.SizeType]::Absolute, 58)))

$labelCamera = New-ValueRow $metrics 'Camera'
$labelFrame = New-ValueRow $metrics 'Frame'
$labelJpeg = New-ValueRow $metrics 'JPEG'
$labelFps = New-ValueRow $metrics 'Display rate'
$labelScore = New-ValueRow $metrics 'Detector score'
$labelBox = New-ValueRow $metrics 'Face box'
$labelDetTime = New-ValueRow $metrics 'Detect pipeline time'
$labelIdTime = New-ValueRow $metrics 'Descriptor time'
$labelTotalTime = New-ValueRow $metrics 'Total time'
$labelNorm = New-ValueRow $metrics 'Embedding norm / LBP distance'
$labelReference = New-ValueRow $metrics 'Reference'
$labelSimilarity = New-ValueRow $metrics 'Similarity'
$labelMatchCount = New-ValueRow $metrics 'Independent matches'
$labelNoMatchCount = New-ValueRow $metrics 'No matches'
$labelBestSimilarity = New-ValueRow $metrics 'Best similarity'
$labelStatusCode = New-ValueRow $metrics 'Status code'

$textLog = New-Object Windows.Forms.RichTextBox
$textLog.Dock = 'Fill'
$textLog.ReadOnly = $true
$textLog.WordWrap = $false
$textLog.Font = New-Object Drawing.Font('Consolas', 9)
$textLog.BackColor = [Drawing.Color]::FromArgb(20, 22, 24)
$textLog.ForeColor = [Drawing.Color]::Gainsboro
$root.Controls.Add($textLog, 0, 2)

$timer = New-Object Windows.Forms.Timer
$timer.Interval = 30
$timer.Add_Tick({
    if ($null -ne $script:Serial -and $script:Serial.IsOpen) {
        try {
            while ($script:Serial.BytesToRead -gt 0) {
                $count = [Math]::Min($script:Serial.BytesToRead, 65536)
                $bytes = New-Object byte[] $count
                try {
                    $read = $script:Serial.Read($bytes, 0, $count)
                } catch [System.TimeoutException] {
                    # BytesToRead can grow while a packet is arriving. A short
                    # read is normal here; keep the port open and try next tick.
                    break
                }
                if ($read -gt 0) {
                    if ($read -ne $bytes.Length) { $bytes = $bytes[0..($read - 1)] }
                    $script:Buffer.AddRange([byte[]]$bytes)
                    $script:RxBytes += $read
                }
            }
            Parse-Frames
        } catch {
            Add-LogLine ('serial error: ' + $_.Exception.Message)
            Close-Serial
        }
    }
})

$buttonConnect.Add_Click({
    if ($null -ne $script:Serial -and $script:Serial.IsOpen) { Close-Serial } else { Open-Serial }
})
$buttonEnroll.Add_Click({ Send-FaceCommand ('ENROLL {0}' -f [uint16]$numericId.Value) })
$buttonDeleteEnroll.Add_Click({ Send-FaceCommand ('DELETE {0}' -f [uint16]$numericId.Value) })
$buttonListEnroll.Add_Click({ Send-FaceCommand 'LIST' })
$buttonClearEnroll.Add_Click({ Send-FaceCommand 'CLEAR' })
$buttonSave.Add_Click({
    if ($null -eq $script:CurrentBitmap) { return }
    $dialog = New-Object Windows.Forms.SaveFileDialog
    $dialog.Filter = 'JPEG image|*.jpg'
    $dialog.FileName = 'face_' + [DateTime]::Now.ToString('yyyyMMdd_HHmmss') + '.jpg'
    if ($dialog.ShowDialog() -eq [Windows.Forms.DialogResult]::OK) {
        $script:CurrentBitmap.Save($dialog.FileName, [Drawing.Imaging.ImageFormat]::Jpeg)
        Add-LogLine ('saved ' + $dialog.FileName)
    }
    $dialog.Dispose()
})
$form.Add_Shown({ if ($script:AutoOpenRequested) { Open-Serial } })
$form.Add_FormClosing({
    $timer.Stop()
    Close-Serial
    if ($null -ne $script:CurrentBitmap) {
        $script:CurrentBitmap.Dispose()
        $script:CurrentBitmap = $null
    }
})

$timer.Start()
[void]$form.ShowDialog()
