import streamlit as st
import numpy as np
import pandas as pd
import plotly.graph_objects as go
import plotly.express as px
from scipy.stats import norm
import math

# Black-Scholes pricing functions
def black_scholes_call(S, K, T, r, sigma):
    """Calculate Black-Scholes call option price"""
    d1 = (np.log(S / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * np.sqrt(T))
    d2 = d1 - sigma * np.sqrt(T)
    
    call_price = S * norm.cdf(d1) - K * np.exp(-r * T) * norm.cdf(d2)
    return call_price

def black_scholes_put(S, K, T, r, sigma):
    """Calculate Black-Scholes put option price"""
    d1 = (np.log(S / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * np.sqrt(T))
    d2 = d1 - sigma * np.sqrt(T)
    
    put_price = K * np.exp(-r * T) * norm.cdf(-d2) - S * norm.cdf(-d1)
    return put_price

def calculate_greeks(S, K, T, r, sigma, option_type='call'):
    """Calculate option Greeks"""
    d1 = (np.log(S / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * np.sqrt(T))
    d2 = d1 - sigma * np.sqrt(T)
    
    # Delta
    if option_type == 'call':
        delta = norm.cdf(d1)
    else:
        delta = norm.cdf(d1) - 1
    
    # Gamma
    gamma = norm.pdf(d1) / (S * sigma * np.sqrt(T))
    
    # Theta
    if option_type == 'call':
        theta = (-S * norm.pdf(d1) * sigma / (2 * np.sqrt(T)) 
                - r * K * np.exp(-r * T) * norm.cdf(d2)) / 365
    else:
        theta = (-S * norm.pdf(d1) * sigma / (2 * np.sqrt(T)) 
                + r * K * np.exp(-r * T) * norm.cdf(-d2)) / 365
    
    # Vega
    vega = S * norm.pdf(d1) * np.sqrt(T) / 100
    
    # Rho
    if option_type == 'call':
        rho = K * T * np.exp(-r * T) * norm.cdf(d2) / 100
    else:
        rho = -K * T * np.exp(-r * T) * norm.cdf(-d2) / 100
    
    return {'delta': delta, 'gamma': gamma, 'theta': theta, 'vega': vega, 'rho': rho}

# Streamlit app
st.set_page_config(page_title="Black-Scholes Options Pricer", layout="wide")
st.title("🎯 Black-Scholes Options Pricer with P&L Heatmap")

# Sidebar for inputs
st.sidebar.header("Option Parameters")

# Input parameters
S = st.sidebar.number_input("Current Stock Price ($)", value=100.0, min_value=0.01, step=1.0)
K = st.sidebar.number_input("Strike Price ($)", value=100.0, min_value=0.01, step=1.0)
T = st.sidebar.number_input("Time to Expiration (years)", value=0.25, min_value=0.001, max_value=5.0, step=0.01)
r = st.sidebar.number_input("Risk-free Rate (%)", value=5.0, min_value=0.0, max_value=20.0, step=0.1) / 100
sigma = st.sidebar.number_input("Volatility (%)", value=20.0, min_value=0.1, max_value=100.0, step=1.0) / 100
option_type = st.sidebar.selectbox("Option Type", ["call", "put"])

# Calculate option price and Greeks
if option_type == "call":
    option_price = black_scholes_call(S, K, T, r, sigma)
else:
    option_price = black_scholes_put(S, K, T, r, sigma)

greeks = calculate_greeks(S, K, T, r, sigma, option_type)

# Main layout
col1, col2 = st.columns([1, 2])

with col1:
    st.header("Option Pricing")
    st.metric("Option Price", f"${option_price:.4f}")
    
    st.subheader("Greeks")
    st.metric("Delta", f"{greeks['delta']:.4f}")
    st.metric("Gamma", f"{greeks['gamma']:.4f}")
    st.metric("Theta", f"{greeks['theta']:.4f}")
    st.metric("Vega", f"{greeks['vega']:.4f}")
    st.metric("Rho", f"{greeks['rho']:.4f}")

with col2:
    st.header("Enhanced P&L Heatmap")
    
    # Heatmap parameters
    st.subheader("Heatmap Settings")
    col2a, col2b, col2c = st.columns(3)
    
    with col2a:
        spot_range = st.slider("Stock Price Range (%)", -80, 80, (-40, 40))
        vol_range = st.slider("Volatility Range (%)", -70, 200, (-30, 50))
        grid_size = st.slider("Grid Resolution", 15, 100, 30)
    
    with col2b:
        position = st.selectbox("Position", ["Long", "Short"])
        heatmap_type = st.selectbox("Heatmap Type", ["P&L", "Option Price", "% P&L"])
        colorscale = st.selectbox("Color Scheme", 
                                ["RdYlGn", "Viridis", "Plasma", "RdBu", "Spectral", "Turbo"])
    
    with col2c:
        show_contours = st.checkbox("Show Contour Lines", value=True)
        show_breakeven = st.checkbox("Show Breakeven Line", value=True)
        show_moneyness = st.checkbox("Show Moneyness Lines", value=True)
    
    # Create heatmap data
    spot_min = S * (1 + spot_range[0] / 100)
    spot_max = S * (1 + spot_range[1] / 100)
    vol_min = max(0.01, sigma * (1 + vol_range[0] / 100))  # Ensure vol doesn't go negative
    vol_max = sigma * (1 + vol_range[1] / 100)
    
    spot_prices = np.linspace(spot_min, spot_max, grid_size)
    volatilities = np.linspace(vol_min, vol_max, grid_size)
    
    # Calculate matrices
    pnl_matrix = np.zeros((len(volatilities), len(spot_prices)))
    price_matrix = np.zeros((len(volatilities), len(spot_prices)))
    
    for i, vol in enumerate(volatilities):
        for j, spot in enumerate(spot_prices):
            if option_type == "call":
                new_price = black_scholes_call(spot, K, T, r, vol)
            else:
                new_price = black_scholes_put(spot, K, T, r, vol)
            
            price_matrix[i, j] = new_price
            
            if position == "Long":
                pnl = new_price - option_price
            else:
                pnl = option_price - new_price
            
            pnl_matrix[i, j] = pnl
    
    # Select data based on heatmap type
    if heatmap_type == "P&L":
        z_data = pnl_matrix
        colorbar_title = "P&L ($)"
        zmid = 0
    elif heatmap_type == "Option Price":
        z_data = price_matrix
        colorbar_title = "Option Price ($)"
        zmid = None
    else:  # % P&L
        z_data = (pnl_matrix / option_price) * 100
        colorbar_title = "P&L (%)"
        zmid = 0
    
    # Create enhanced heatmap
    fig = go.Figure()
    
    # Add heatmap
    heatmap = go.Heatmap(
        z=z_data,
        x=spot_prices,
        y=volatilities * 100,
        colorscale=colorscale,
        zmid=zmid,
        colorbar=dict(
            title=colorbar_title,
            thickness=15,
            len=0.7
        ),
        hovertemplate='<b>Stock Price:</b> $%{x:.2f}<br>' +
                      '<b>Volatility:</b> %{y:.1f}%<br>' +
                      f'<b>{colorbar_title}:</b> %{{z:.3f}}<br>' +
                      '<extra></extra>',
        showscale=True
    )
    
    fig.add_trace(heatmap)
    
    # Add contour lines if selected
    if show_contours:
        contour = go.Contour(
            z=z_data,
            x=spot_prices,
            y=volatilities * 100,
            showscale=False,
            contours=dict(
                coloring='lines',
                showlabels=True,
                labelfont=dict(size=10, color='white')
            ),
            line=dict(width=1, color='white'),
            opacity=0.6,
            hoverinfo='skip'
        )
        fig.add_trace(contour)
    
    # Add breakeven line if selected and P&L heatmap
    if show_breakeven and heatmap_type in ["P&L", "% P&L"]:
        # Find breakeven points (where P&L ≈ 0)
        breakeven_spots = []
        breakeven_vols = []
        
        for i, vol in enumerate(volatilities):
            for j, spot in enumerate(spot_prices):
                if abs(pnl_matrix[i, j]) < 0.01:  # Close to breakeven
                    breakeven_spots.append(spot)
                    breakeven_vols.append(vol * 100)
        
        if breakeven_spots:
            fig.add_scatter(
                x=breakeven_spots,
                y=breakeven_vols,
                mode='markers',
                marker=dict(size=3, color='yellow', symbol='circle'),
                name='Breakeven Points',
                hovertemplate='<b>Breakeven</b><br>Price: $%{x:.2f}<br>Vol: %{y:.1f}%<extra></extra>'
            )
    
    # Add moneyness lines if selected
    if show_moneyness:
        # ATM line (S = K)
        fig.add_vline(x=K, line_dash="dash", line_color="cyan", line_width=2,
                     annotation_text="ATM", annotation_position="top")
        
        # ITM/OTM boundaries
        if option_type == "call":
            fig.add_vline(x=K*0.9, line_dash="dot", line_color="orange", line_width=1,
                         annotation_text="10% OTM", annotation_position="bottom left")
            fig.add_vline(x=K*1.1, line_dash="dot", line_color="green", line_width=1,
                         annotation_text="10% ITM", annotation_position="bottom right")
        else:
            fig.add_vline(x=K*1.1, line_dash="dot", line_color="orange", line_width=1,
                         annotation_text="10% OTM", annotation_position="bottom right")
            fig.add_vline(x=K*0.9, line_dash="dot", line_color="green", line_width=1,
                         annotation_text="10% ITM", annotation_position="bottom left")
    
    # Add current position marker
    fig.add_scatter(
        x=[S],
        y=[sigma * 100],
        mode='markers',
        marker=dict(size=20, color='blue', symbol='x', line=dict(width=3, color='white')),
        name='Current Position',
        hovertemplate='<b>Current Position</b><br>Price: $%{x:.2f}<br>Vol: %{y:.1f}%<extra></extra>'
    )
    
    # Enhanced layout
    fig.update_layout(
        title=dict(
            text=f"{position} {option_type.title()} Option {heatmap_type} Heatmap",
            x=0.5,
            font=dict(size=16)
        ),
        xaxis=dict(
            title="Stock Price ($)",
            showgrid=True,
            gridwidth=1,
            gridcolor='rgba(128,128,128,0.2)'
        ),
        yaxis=dict(
            title="Volatility (%)",
            showgrid=True,
            gridwidth=1,
            gridcolor='rgba(128,128,128,0.2)'
        ),
        height=600,
        plot_bgcolor='rgba(0,0,0,0)',
        paper_bgcolor='rgba(0,0,0,0)',
        font=dict(size=12),
        showlegend=True,
        legend=dict(
            yanchor="top",
            y=0.99,
            xanchor="left",
            x=1.02
        )
    )
    
    st.plotly_chart(fig, use_container_width=True)
    
    # Add summary statistics
    col2d, col2e, col2f = st.columns(3)
    
    with col2d:
        max_profit = np.max(pnl_matrix)
        max_loss = np.min(pnl_matrix)
        st.metric("Max Profit", f"${max_profit:.2f}")
        st.metric("Max Loss", f"${max_loss:.2f}")
    
    with col2e:
        profit_prob = np.sum(pnl_matrix > 0) / pnl_matrix.size * 100
        st.metric("Profit Probability", f"{profit_prob:.1f}%")
        
        current_pnl = 0  # Current position P&L is 0
        st.metric("Current P&L", f"${current_pnl:.2f}")
    
    with col2f:
        # Risk metrics
        pnl_std = np.std(pnl_matrix)
        pnl_mean = np.mean(pnl_matrix)
        if pnl_std > 0:
            sharpe_approx = pnl_mean / pnl_std
        else:
            sharpe_approx = 0
        
        st.metric("P&L Std Dev", f"${pnl_std:.2f}")
        st.metric("Risk-Reward Ratio", f"{sharpe_approx:.2f}")

# Additional analysis
st.header("Sensitivity Analysis")

col3, col4 = st.columns(2)

with col3:
    st.subheader("Price vs Stock Price")
    spot_range_analysis = np.linspace(S * 0.7, S * 1.3, 100)
    
    if option_type == "call":
        prices = [black_scholes_call(spot, K, T, r, sigma) for spot in spot_range_analysis]
    else:
        prices = [black_scholes_put(spot, K, T, r, sigma) for spot in spot_range_analysis]
    
    fig_price = go.Figure()
    fig_price.add_trace(go.Scatter(x=spot_range_analysis, y=prices, mode='lines', name='Option Price'))
    fig_price.add_vline(x=S, line_dash="dash", line_color="red", annotation_text="Current Price")
    fig_price.update_layout(xaxis_title="Stock Price ($)", yaxis_title="Option Price ($)")
    st.plotly_chart(fig_price, use_container_width=True)

with col4:
    st.subheader("Price vs Volatility")
    vol_range_analysis = np.linspace(0.05, 1.0, 100)
    
    if option_type == "call":
        vol_prices = [black_scholes_call(S, K, T, r, vol) for vol in vol_range_analysis]
    else:
        vol_prices = [black_scholes_put(S, K, T, r, vol) for vol in vol_range_analysis]
    
    fig_vol = go.Figure()
    fig_vol.add_trace(go.Scatter(x=vol_range_analysis * 100, y=vol_prices, mode='lines', name='Option Price'))
    fig_vol.add_vline(x=sigma * 100, line_dash="dash", line_color="red", annotation_text="Current Vol")
    fig_vol.update_layout(xaxis_title="Volatility (%)", yaxis_title="Option Price ($)")
    st.plotly_chart(fig_vol, use_container_width=True)