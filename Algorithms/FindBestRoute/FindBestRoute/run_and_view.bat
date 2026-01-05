@echo off

tsp.exe
if errorlevel 1 (
  echo Program returned error.
  pause
  exit /b 1
)

D:\Graphviz\Graphviz-14.0.4-win32\bin\dot -Tsvg graph.dot -o graph.svg
if errorlevel 1 (
  echo Graphviz conversion failed for graph.dot.
  pause
  exit /b 1
)
D:\Graphviz\Graphviz-14.0.4-win32\bin\dot -Tsvg graph_highlighted.dot -o graph_highlighted.svg
if errorlevel 1 (
  echo Graphviz conversion failed for graph_highlighted.dot.
  pause
  exit /b 1
)

start "" "%CD%\graph.svg"
start "" "%CD%\graph_highlighted.svg"
