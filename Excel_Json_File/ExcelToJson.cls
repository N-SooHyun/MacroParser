Sub ExportToJson()
    Dim lastRow As Long
    Dim jsonFile As Object
    Dim jsonData As String
    Dim seq As Integer
    Dim i As Long

    ' 마지막 행을 구함
    lastRow = Cells(Rows.Count, "B").End(xlUp).Row

    ' JSON 데이터의 구조 시작
    jsonData = "{""Excel"":["

    ' Seq와 Call을 순차적으로 JSON 형식에 맞게 변환
    For i = 2 To lastRow
        If Not IsEmpty(Cells(i, 2).Value) Then ' B열(Call) 값이 있으면
            jsonData = jsonData & "{""" & Cells(i, 1).Value & """: """ & Cells(i, 2).Value & """},"
        End If
    Next i

    ' 마지막 쉼표 제거
    If Right(jsonData, 1) = "," Then
        jsonData = Left(jsonData, Len(jsonData) - 1)
    End If

    ' JSON 데이터 끝
    jsonData = jsonData & "]}"

    ' JSON 파일로 저장
    Set jsonFile = CreateObject("Scripting.FileSystemObject")
    Dim filePath As String
    filePath = "..\Desktop\Company\Macro_Func\MacroParserToCpp\MacroParser\Excel_Json_File\MacroCallData.json" ' 원하는 파일 경로로 변경

    Dim file As Object
    Set file = jsonFile.CreateTextFile(filePath, True)
    file.Write jsonData
    file.Close

    MsgBox "JSON 파일이 생성되었습니다!"
End Sub

