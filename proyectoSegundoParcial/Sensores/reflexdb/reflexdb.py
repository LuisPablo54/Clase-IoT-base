"""Welcome to Reflex! This file outlines the steps to create a basic app."""

import reflex as rx
from rxconfig import config
from modelo.conexion import Conexion
from modelo.productoBase import ProductoBase

class State(rx.State):
    producto: str = ProductoBase()
    
    def test_conexion(self):
        conexion = Conexion()
        conexion.establecerConexion()
        conexion.cerrarConexion()
        
    def listar_producto(self):
        self.producto.listarProducto()
        
    def insertar_producto(self):
        self.producto.producto.clave = "SKU-40"
        self.producto.producto.descripcion = "Papas"
        self.producto.producto.existencia = 10
        self.producto.producto.precio = 23
        self.producto.guardarProducto()
        
    def modificar_producto(self):
        self.producto.producto.id_producto = 120  # Ejemplo de ID
        self.producto.producto.clave = "SKU-40"
        self.producto.producto.descripcion = "Papas Modificadas"
        self.producto.producto.existencia = 15
        self.producto.producto.precio = 25
        self.producto.actualizarProducto()
        
    def eliminar_producto(self):
        self.producto.producto.id_producto = 22
        self.producto.eliminarProducto()
        
def index() -> rx.Component:
    # Welcome Page (Index)
    return rx.container(
      rx.center(
          rx.hstack(
              rx.button('Conexión',
                        on_click=State.test_conexion),
              rx.button('Listar productos',
                        on_click=State.listar_producto), 
              rx.button('Agregar producto',
                        on_click=State.insertar_producto),     
              rx.button('Modificar producto',
                        on_click=State.modificar_producto),   
              rx.button('Eliminar producto',
                        on_click=State.eliminar_producto),       
          )
      )
    )

app = rx.App()
app.add_page(index)
