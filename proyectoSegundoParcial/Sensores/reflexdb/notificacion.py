import reflex as rx 

def notify_component(mensaje: str, icon_notify: str, color: str):
    return rx.callout(mensaje,icon=icon_notify,color_scheme=color, style=style_notify)

style_notify={
    'position':'fixed',
    'top':'0px',
    'right':'0px',
    'margin':'10px 10px'
}