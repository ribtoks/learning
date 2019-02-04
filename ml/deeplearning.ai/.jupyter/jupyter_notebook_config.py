c = get_config()  # get the config object
c.IPKernelApp.pylab = 'inline'  # in-line figure when using Matplotlib
c.NotebookApp.ip = '0.0.0.0'
# do not open a browser window by default when using notebooks
c.NotebookApp.open_browser = False
c.NotebookApp.token = ''  # No token. Always use jupyter over ssh tunnel
c.NotebookApp.notebook_dir = '/notebooks'
# Allow to run Jupyter from root user inside Docker container
c.NotebookApp.allow_root = False
c.NotebookApp.allow_origin = '*'
c.NotebookApp.tornado_settings = {
    'headers': {
        'Content-Security-Policy': "frame-ancestors 'self' *"
    }
}
