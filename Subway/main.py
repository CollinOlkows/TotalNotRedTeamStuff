import tkinter as tk
from PIL import Image, ImageTk
import os

dir_path = os.path.dirname(os.path.realpath(__file__))

# Define the path to your GIF file
gif_path = dir_path+"/sub.gif"

# Create the main window
root = tk.Tk()

# Remove the title bar and window controls (minimize, maximize, close)
root.overrideredirect(True)

# Open the GIF using Pillow
image = Image.open(gif_path)

# Get the dimensions of the image
width, height = image.size

# Get the screen width and height (resolution)
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

# Position the window at the top-right corner
x_position = screen_width - width  # x = screen width - image width
y_position = 0  # y = 0 (top of the screen)

# Set the window size to match the image size and position it in the top-right corner
root.geometry(f"{width}x{height}+{x_position}+{y_position}")

# Create a list of frames from the GIF
frames = []
for frame in range(0, image.n_frames):
    image.seek(frame)
    frames.append(ImageTk.PhotoImage(image.copy()))

# Function to update the image in the window
def update_image(frame_index):
    # Update the label with the new image frame
    label.config(image=frames[frame_index])
    
    # Update the frame index and call the function again
    next_frame = (frame_index + 1) % len(frames)
    
    # Speed up the animation by reducing the delay
    root.after(50, update_image, next_frame)  # 50ms per frame (faster speed)

# Convert the first frame to a Tkinter-compatible photo object
label = tk.Label(root, image=frames[0])
label.pack()

# Start the animation
update_image(0)

# Run the Tkinter main loop
root.mainloop()
