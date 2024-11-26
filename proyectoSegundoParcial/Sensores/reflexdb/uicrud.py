import reflex as rx
from modelo.producto import Producto
from .notificacion import notify_component
import asyncio
from modelo.productoBase import ProductoBase


class CrudState(rx.State):
    lista_productos: list[Producto]
    datos: dict
    mensaje: str = ''
    clave_buscar: str = ""

    async def handleNotify(self):
        async with self:
            await asyncio.sleep(3)
            self.mensaje = ''

    @rx.background
    async def set_clave_buscar(self, value: str):
        async with self:
            self.clave_buscar = value

    @rx.background
    async def obtener_todos_productos(self):
        async with self:
            try:
                producto_base = ProductoBase()
                productos = producto_base.listarProducto()
                # Mapeo correcto de columnas
                self.lista_productos = [
                    {
                        'clave': row[1],
                        'descripcion': row[2],
                        'existencia': row[3],
                        'precio': float(row[4]) if row[4] is not None else 0.0,
                    }
                    for row in productos
                ]
            except Exception as e:
                print(f"Error al obtener productos: {e}")
                self.mensaje = "Error al obtener productos de la base de datos."

    @rx.background
    async def guardar_producto(self, datos: dict):
        try:
            producto_base = ProductoBase()
            producto_base.guardarProducto(datos)  
            async with self:
                self.mensaje = "El producto ha sido registrado."
            
            productos = producto_base.listarProducto()
            async with self:
                self.lista_productos = [
                    {
                        'clave': row[1],
                        'descripcion': row[2],
                        'existencia': row[3],
                        'precio': float(row[4]) if row[4] is not None else 0.0,
                    }
                    for row in productos
                ]
        except Exception as e:
            print(f"Error al guardar producto: {e}")
            async with self:
                self.mensaje = "Error al guardar el producto."

        await asyncio.sleep(3)
        async with self:
            self.mensaje = ""


    @rx.background
    async def eliminar_producto(self, clave: str):
        try:
            producto_base = ProductoBase()
            producto_base.eliminarProducto(clave)
            
            # Actualizar la lista de productos después de eliminar
            productos = producto_base.listarProducto()
            async with self:
                self.lista_productos = [
                    {
                        'clave': row[1],
                        'descripcion': row[2],
                        'existencia': row[3],
                        'precio': float(row[4]) if row[4] is not None else 0.0,
                    }
                    for row in productos
                ]
                self.mensaje = f"Producto con clave {clave} eliminado correctamente."
        except Exception as e:
            print(f"Error al eliminar producto: {e}")
            async with self:
                self.mensaje = "Error al eliminar el producto."

        await asyncio.sleep(3)
        async with self:
            self.mensaje = ""

    @rx.background
    async def modificar_producto(self, datos: dict):
        try:
            # Validar que los datos contienen la clave
            if not datos.get("clave"):
                raise ValueError("La clave es obligatoria para modificar el producto.")

            # Obtener los datos actuales del producto a modificar
            producto_actual = next(
                (p for p in self.lista_productos if p["clave"] == datos["clave"]), None
            )
            if not producto_actual:
                raise ValueError(f"No se encontró el producto con clave {datos['clave']}.")

            # Conservar valores originales si los campos están vacíos
            datos["descripcion"] = datos.get("descripcion", "").strip() or producto_actual["descripcion"]
            datos["existencia"] = (
                int(datos["existencia"]) if datos.get("existencia") else producto_actual["existencia"]
            )
            datos["precio"] = (
                float(datos["precio"]) if datos.get("precio") else producto_actual["precio"]
            )

            producto_base = ProductoBase()
            producto_base.actualizarProducto(datos) 

          
            async with self:
                self.mensaje = f"Producto con clave {datos['clave']} modificado correctamente."

            productos = producto_base.listarProducto()
            async with self:
                self.lista_productos = [
                    {
                        "clave": row[1],
                        "descripcion": row[2],
                        "existencia": row[3],
                        "precio": float(row[4]) if row[4] is not None else 0.0,
                    }
                    for row in productos
                ]
        except Exception as e:
            print(f"Error al modificar producto: {e}")
            async with self:
                self.mensaje = "Error al modificar el producto."

        await asyncio.sleep(3)
        async with self:
            self.mensaje = ""

    @rx.background
    async def buscar_producto(self):
        async with self:
            try:
                if not self.clave_buscar.strip(): 
                 
                    yield CrudState.obtener_todos_productos()
                    self.mensaje = "" 
                else:
                    producto_base = ProductoBase()
                    productos = producto_base.buscarProducto(clave=self.clave_buscar)
                    if productos:
                        self.lista_productos = [
                            {
                                'clave': row[1],  # Clave (SKU)
                                'descripcion': row[2],
                                'existencia': row[3],
                                'precio': float(row[4]) if row[4] is not None else 0.0,
                            }
                            for row in productos
                        ]
                        self.mensaje = ""  
                    else:
                        self.lista_productos = []  
                        self.mensaje = "No se encontraron productos."
            except Exception as e:
                print(f"Error al buscar producto: {e}")
                self.mensaje = "Error al buscar el producto."


# Vista - GUI
@rx.page(route='/uicrud', title="CRUD", on_load=CrudState.obtener_todos_productos)
def uicrud() -> rx.Component:
    return rx.flex(
        rx.center(
            rx.heading('Catálogo de productos', align='center'),
            rx.hstack(
                componentes_buscar(),
                cuadro_dialogo_crear()
            ),
            tabla_producto(CrudState.lista_productos),
            rx.cond(
                CrudState.mensaje != '',
                notify_component(CrudState.mensaje, 'info', 'blue')
            ),
            direction='column',
            style={'width': '60vw', "margin": 'auto'}
        ),
    )

def componentes_buscar() -> rx.Component:
    return rx.hstack(
        rx.input(
            placeholder='Clave del producto',
            name='clave',
            id='clave-buscar',
            on_change=lambda value: CrudState.set_clave_buscar(value), 
        ),
        rx.button(
            'Buscar producto',
            on_click=CrudState.buscar_producto,  
        ),
    )

def tabla_producto(lista_producto: list[Producto]) -> rx.Component:
    return rx.table.root(
        rx.table.header(
            rx.table.row(
                rx.table.column_header_cell('Clave'),
                rx.table.column_header_cell('Descripcion'),
                rx.table.column_header_cell('Existencia'),
                rx.table.column_header_cell('Precio'),
                rx.table.column_header_cell('Accion'),
            )
        ),
        rx.table.body(
            rx.foreach(lista_producto, fila_tabla)
        )
    )

def fila_tabla(producto: Producto) -> rx.Component:
    return rx.table.row(
        rx.table.cell(producto.clave),          
        rx.table.cell(producto.descripcion),    
        rx.table.cell(producto.existencia),     
        rx.table.cell(f"{producto.precio:.2f}"),  
        rx.table.cell(rx.hstack(
            cuadro_dialogo_eliminar(producto.clave),  
            cuadro_dialogo_modificar(producto)       
        )),
    )


def cuadro_dialogo_eliminar(clave: str) -> rx.Component:
    return rx.dialog.root(
        rx.dialog.trigger(rx.button(rx.icon('trash-2'), color_scheme='red')),
        rx.dialog.content(
            rx.flex(
                rx.dialog.title('Eliminar producto'),
                rx.dialog.description(f'¿Estás seguro de eliminar el producto con clave "{clave}"?'),
                justify='center',
                align='center',
                direction='column',
            ),
            rx.flex(
                rx.dialog.close(rx.button('Cancelar', color_scheme='gray', variant='soft')),
                rx.dialog.close(rx.button('Confirmar',
                                          on_click=lambda: CrudState.eliminar_producto(clave),  
                                          color_scheme='red', variant='solid')),
                spacing='3',
                margin_top='16px',
                justify='end'
            ),
            style={'width': '300px'}
        )
    )



def cuadro_dialogo_modificar(producto: Producto) -> rx.Component:
    return rx.dialog.root(
        rx.dialog.trigger(rx.button(rx.icon('pencil-ruler'), color_scheme='blue')),
        rx.dialog.content(
            rx.flex(
                rx.dialog.title('Modificar producto'),
                form_modificar_producto(producto),
                justify='center',
                align='center',
                direction='column',
            ),
            rx.flex(
                rx.dialog.close(rx.button('Cancelar', color_scheme='red', variant='soft')),
                spacing='3',
                margin_top='16px',
                justify='end'
            ),
            style={'width': '300px'}
        )
    )

def cuadro_dialogo_crear() -> rx.Component:
    return rx.dialog.root(
        rx.dialog.trigger(rx.button('Crear Producto')),
        rx.dialog.content(
            rx.flex(
                rx.dialog.title('Crear producto'),
                form_captura_producto(),
                justify='center',
                align='center',
                direction='column',
            ),
            rx.flex(
                rx.dialog.close(rx.button('Cancelar', color_scheme='red', variant='soft')),
                spacing='3',
                margin_top='16px',
                justify='end'
            ),
            style={'width': '300px'}
        )
    )

def form_captura_producto() -> rx.Component:
    return rx.form(
        rx.vstack(
            rx.input(placeholder='Clave', name='clave'),
            rx.input(placeholder='Descripcion', name='descripcion'),
            rx.input(placeholder='Existencia', name='existencia'),
            rx.input(placeholder='Precio', name='precio'),
            rx.dialog.close(rx.button('Guardar', type='submit'))
        ),
        on_submit=CrudState.guardar_producto,  
    )


def form_modificar_producto(producto: Producto) -> rx.Component:
    return rx.form(
        rx.vstack(
            rx.input(value=producto.clave, name='clave', read_only=True),  
            rx.input(placeholder='Nueva descripción', name='descripcion'),  
            rx.input(placeholder='Nueva existencia', name='existencia'),  
            rx.input(placeholder='Nuevo precio', name='precio'),  
            rx.dialog.close(rx.button('Modificar', type='submit'))
        ),
        on_submit=lambda datos: CrudState.modificar_producto(datos),  
    )



