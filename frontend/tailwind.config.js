module.exports = {
  content: ['./src/**/*.{js,jsx}', './public/index.html'],
  theme: {
    extend: {
      colors: {
        'bio-bg': '#0a0e1a',
        'bio-card': '#1a1f35',
        'bio-border': '#2a3050',
        'bio-cyan': '#00d4ff',
        'bio-green': '#00ff88',
        'bio-red': '#ff4757',
        'bio-amber': '#ffb347',
        'bio-purple': '#a78bfa',
      },
      fontFamily: {
        'mono': ['"JetBrains Mono"', 'monospace'],
        'sans': ['"Plus Jakarta Sans"', 'sans-serif'],
      }
    }
  },
  plugins: [],
}
