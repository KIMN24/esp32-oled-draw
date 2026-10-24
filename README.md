# ESP32 OLED Draw

Página web para enviar dibujos de 128 × 64 píxeles a Firebase Realtime Database y mostrarlos en un ESP32 con OLED SSD1306.

## Contenido
- `web/index.html`: página web lista para GitHub Pages.
- `firmware/`: sketch Arduino para ESP32.
- `database.rules.json`: reglas de Firebase Realtime Database (no se despliegan automáticamente).
- `.github/workflows/pages.yml`: despliegue de `web/` a GitHub Pages.

## Publicación
1. Crear un repositorio público `esp32-oled-draw` en GitHub.
2. Subir **el contenido de este ZIP**, conservando `.github/`.
3. En **Settings > Pages > Build and deployment**, seleccionar **GitHub Actions**.
4. Esperar a que termine el workflow **Publicar web en GitHub Pages**.
5. Abrir `https://TU_USUARIO.github.io/esp32-oled-draw/`.

## Seguridad
El firmware de este repositorio NO contiene la contraseña del usuario ESP32: debes configurarla en tu copia local. No subas tu sketch con credenciales reales. Cambia la contraseña previamente expuesta en Firebase Authentication.

La API key pública de Firebase en la página no es una contraseña: la seguridad depende de Authentication, las reglas de Realtime Database y las restricciones adecuadas de la API key.
