"""Builds ESP32/Smart_Energy_Monitor/dashboard.h from the files in Dashboard/.

Run from the repository root:  python tools/generate_dashboard_header.py
Re-run it whenever index.html, style.css or script.js change.
"""
from pathlib import Path

root = Path(__file__).resolve().parent.parent
dash = root / "Dashboard"
out = root / "ESP32" / "Smart_Energy_Monitor" / "dashboard.h"

html = (dash / "index.html").read_text(encoding="utf-8")
css = (dash / "style.css").read_text(encoding="utf-8")
js = (dash / "script.js").read_text(encoding="utf-8")

html = html.replace('<link rel="stylesheet" href="style.css" />', "<style>\n" + css + "\n</style>")
html = html.replace('<script src="script.js"></script>', "<script>\n" + js + "\n</script>")

if ')rawliteral"' in html:
    raise SystemExit("Dashboard content contains the raw string end marker")

out.write_text(
    "// Generated from the Dashboard/ folder by tools/generate_dashboard_header.py. Do not edit by hand.\n"
    "#pragma once\n"
    "#include <pgmspace.h>\n\n"
    'const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(' + html + ')rawliteral";\n',
    encoding="utf-8",
)
print("Wrote", out, len(html), "bytes of HTML")
