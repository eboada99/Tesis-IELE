import dash
from dash import dcc, html
from dash.dependencies import Input, Output, State
import plotly.express as px
import plotly.graph_objects as go
import pandas as pd
import numpy as np
from pgmpy.models import BayesianNetwork, BayesianModel
from pgmpy.inference import VariableElimination
from pgmpy.estimators import MaximumLikelihoodEstimator, PC
from pgmpy.estimators import HillClimbSearch
from pgmpy.estimators import K2Score, BicScore
from plotly.subplots import make_subplots
import plotly.graph_objs as go
import plotly.io as pio
pio.renderers.default = "browser"
from sqlalchemy import create_engine, text
from pgmpy.readwrite import BIFReader
from statistics import mode
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import threading

# DATOS --------------------------------------------------------------------------------------------------------------------------------------------------------------

# Data lists for plotting
pyr_data = [] 
t_data = []
v_data = []
i_data = []
P_data = []
timestamps = []
data_lock = threading.Lock()


# Crear una figura para cada conjunto de datos
def create_figure(x_data, y_data, title, yaxis_title):
    fig = go.Figure([go.Scatter(x=x_data, y=y_data)])
    fig.update_layout(
        title=title,
        xaxis_title="Tiempo",
        yaxis_title=yaxis_title,
        xaxis=dict(type='date', gridcolor='lightgrey', linecolor='lightgrey'),
        yaxis=dict(gridcolor='lightgrey', linecolor='lightgrey'),
        plot_bgcolor='white',  # Color de fondo de la gráfica
        paper_bgcolor='white',  # Color de fondo alrededor de la gráfica
    )
    fig.update_xaxes(
        tickformat="%d-%m-%Y %H:%M",
        tickangle=45,
        nticks=20
    )
    return fig


# DASH ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets)
server = app.server

imagen2 = "https://uniandes.edu.co/sites/default/files/voces-academia-sociedad-n.jpg"

app.layout = html.Div(style={'overflowY': 'auto'},
    children=[
        html.H1(
            children=[
                "Sistema de monitoreo de la Microred no convencional del edificio Julio Mario Santo Domingo de la Universidad de Los Andes",],
            style={
                "color": "white",
                "backgroundColor": "#ADD4D9",
                "textAlign": "center",
                "padding": "50px",
                "position": "relative",
                "font-size": "40px"}),

        dcc.Tabs(
            id="tabs",
            value="inicio",
            children=[
            # Tab 1: Inicio
                dcc.Tab(style={'backgroundColor':'#FAFAF3', 'fontSize': '18px', "color": "#526771"},
                    label="Inicio",
                    value="inicio",
                    children=[
                        html.Div(
    className="texto",
    style={"flex": "1"},
    children=[
        html.Br(),
        html.Br(),                   
        html.Div(
            style={'display': 'flex', 'justify-content': 'center'},
            children=[
                # Div para el texto, colocado a la izquierda
                html.Div(
                    style={
                        'margin-right': '10px', 
                        'margin-left': '80px',
                        'align-self': 'center',  # Ajuste para centrar verticalmente el texto
                        'width': '50%', 
                        'text-align': 'justify' },
                    children=[
                        html.H6(
                            "Bienvenido al sistema SCADA de monitoreo de la Microred en el Edificio Julio Mario Santo Domingo de la Universidad de Los Andes. Este sistema avanzado proporciona una visión integral de nuestra red de energía renovable, con paneles solares instalados en la azotea del edificio. A través de las pestañas intuitivas, el usuario puede acceder a datos en tiempo real sobre la irradiancia, temperatura, voltaje, corriente y potencia generada por los paneles solares. Esta plataforma asegura una supervisión eficiente y detallada, permitiendo evidenciar el rendimiento energético y contribuir a una gestión sostenible del campus universitario.", 
                            style={"color": "#526771", 'fontSize': '18px'}),
                        html.Br(),
                        html.Br(),
                        html.H6(
                            "Esteban Boada Robayo", 
                            style={"color": "#526771", 'fontSize': '18px'}),    
                            
                            ]),
                # Div para la imagen, colocado a la derecha
                html.Div(
                    style={
                        'display': 'flex', 
                        'align-items': 'center', 
                        'justify-content': 'center',
                        'width': '50%'},
                    children=[
                        html.Img(
                            src=imagen2,
                            style={"width": "400px", "height": "300px"})])])])]),

            # Tab 2:
            dcc.Tab(label="Irradiancia",
                    value="irradiancia", 
                    children=[
                            dcc.Graph(id='graph-irradiance')]),
            
            # Tab 3:
            dcc.Tab(label="Temperatura",
                    value="temperatura", 
                    children=[
                            dcc.Graph(id='graph-temperature')]),
            
            # Tab 4:
            dcc.Tab(label="Voltaje",
                    value="voltaje", 
                    children=[
                            dcc.Graph(id='graph-voltage')]),
            
            # Tab 5:
            dcc.Tab(label="Corriente",
                    value="corriente", 
                    children=[
                            dcc.Graph(id='graph-current')]),

            # Tab 6:
            dcc.Tab(label="Potencia",
                    value="potencia", 
                    children=[
                            dcc.Graph(id='graph-power')])

           ]), 
           dcc.Interval(id='interval-component', interval=1*1000, n_intervals=0)
           ])

# Callbacks para actualizar las gráficas
@app.callback(
    [Output('graph-irradiance', 'figure'),
     Output('graph-temperature', 'figure'), 
     Output('graph-voltage', 'figure'), 
     Output('graph-current', 'figure'), 
     Output('graph-power', 'figure')],
    [Input('interval-component', 'n_intervals')]
)

def update_graphs(n):
    # Read data from the CSV file
    data = pd.read_csv('Datos/sensor_data.csv')
    data['Time'] = pd.to_datetime(data['Time'])  # Convert the time column to datetime

    # Create figures using the data from the CSV file
    fig_irradiance = create_figure(data['Time'], data['PYR'], 'Irradiancia', 'W/m<sup>2</sup>')
    fig_temperature = create_figure(data['Time'], data['T'], 'Temperatura', '°C')
    fig_voltage = create_figure(data['Time'], data['V'], 'Voltaje', 'V')
    fig_current = create_figure(data['Time'], data['I'], 'Corriente', 'A')
    fig_power = create_figure(data['Time'], data['P'], 'Potencia', 'W')

    return fig_irradiance, fig_temperature, fig_voltage, fig_current, fig_power

if __name__ == '__main__':
    app.run_server(debug=True, use_reloader=False, port=9878, host = "0.0.0.0")
